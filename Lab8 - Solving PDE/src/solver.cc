#include "grid.hh"
#include "scheme.hh"

#include <cassert>
#include <mpi.h>

namespace
{

void
solveGridPartial( pde::Grid& grid,
                  pde::Scheme scheme)
{
    int t = grid.getCurrentNode().second;
    grid.setCurrentNode( 0, t);

    int x_size = grid.getSize().first;

    for ( int x = 0; x < x_size; x++ )
    {
        double value = scheme( grid);
        grid.set( value, 0, 1);
        grid.moveCurrentNode();
    }
} // solveGridPartial

void
sendUpdateToProcesses( pde::Grid& grid,
                       int rank)
{
    int t = grid.getCurrentNode().second;

    MPI_Request req{};

    /**
     * First process has phantom left neighbour.
     */
    if ( grid.hasLeftNeighbour() && rank != 0 )
    {
        double grid_value = grid.getLeft( t);
        MPI_Isend(
            &grid_value, 1, MPI_DOUBLE,
            rank - 1, 0,
            MPI_COMM_WORLD, &req
        );
    }

    if ( grid.hasRightNeighbour() )
    {
        MPI_Request req{};

        double grid_value = grid.getRight( t);
        MPI_Isend(
            &grid_value, 1, MPI_DOUBLE,
            rank + 1, 0,
            MPI_COMM_WORLD, &req
        );

    }
} // sendUpdateToProcesses

void
recvUpdateFromProcesses( pde::Grid& grid,
                         int rank)
{
    int t = grid.getCurrentNode().second;

    /**
     * First process has phantom left neighbour.
     */
    int n = 0;

    if ( grid.hasLeftNeighbour() && rank != 0 )
    {
        n += 1;
    }

    if ( grid.hasRightNeighbour() )
    {
        n += 1;
    }

    for ( int i = 0; i < n; i++ )
    {
        double neighbour_value = 0.0;
        MPI_Status status{};

        MPI_Recv(
            &neighbour_value, 1, MPI_DOUBLE,
            MPI_ANY_SOURCE, MPI_ANY_TAG, 
            MPI_COMM_WORLD, &status
        );

        if ( status.MPI_SOURCE < rank )
        {
            grid.getLeftNeighbour()->setRight( neighbour_value, t);
        } else
        {
            grid.getRightNeighbour()->setLeft( neighbour_value, t);
        }
    }
} // recvUpdateFromProcesses

void
syncNeighbourGrid( pde::Grid& grid,
                   int rank)
{
    if ( rank % 2 == 0 )
    {
        sendUpdateToProcesses( grid, rank);
        recvUpdateFromProcesses( grid, rank);
    } else
    {
        recvUpdateFromProcesses( grid, rank);
        sendUpdateToProcesses( grid, rank);
    }
} // syncNeighbourGrid

} // anonymous namespace

namespace pde
{

void
solveGridSeq( pde::Grid& grid,
              pde::Scheme scheme)
{
    grid.setCurrentNode( 0, 0);

    int t_size = grid.getSize().second;

    for ( int t = 0; t < (t_size - 1); t++ )
    {
        solveGridPartial( grid, scheme);
    }
} // solveGridSeq

void
solveGridPar( pde::Grid& grid,
              pde::Scheme scheme,
              int rank)
{
    grid.setCurrentNode( 0, 0);

    int t_size = grid.getSize().second;
    
    for ( int t = 0; t < (t_size - 1); t++ )
    {
        syncNeighbourGrid( grid, rank);
        solveGridPartial( grid, scheme);
    }
} // solveGridPar

} // namespace pde
