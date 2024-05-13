#include "grid.hh"
#include "plot.hh"
#include "drawer.hh"
#include "solver.hh"

#include <cmath>
#include <iostream>
#include <mpi.h>

namespace
{

pde::Grid
createGrid( pde::Grid::Function fn,
            pde::Grid::Function border,
            int x_size, 
            int t_size, 
            int rank)
{
    double h = 0.01;
    double a = 0.001;

    double x_shift = h * (rank * x_size + 1);
    auto shift_fn = [fn, x_shift]( double x, double t) { return fn( x_shift + x, t); };
    pde::Grid grid{ x_size, t_size, h, a, shift_fn};

    for ( int x = 0; x < x_size; x++ )
    {
        grid.setAbs( border( h*(rank * x_size + x + 1), 0), x, 0);
    }

    return grid;
} // createGrid

pde::Grid
createNeighbourGrid( int t_size)
{
    return pde::Grid{ 1, t_size, 0, 0, nullptr};
} // createNeighbourGrid

pde::Grid
createBorderGrid( pde::Grid::Function border,
                  int t_size, 
                  double a)
{
    pde::Grid grid{ 1, t_size, 0, 0, nullptr};

    for ( int t = 0; t < t_size; t++ )
    {
        grid.setAbs( border( 0, a*t), 0, t);
    }

    return grid;
} // createBorderGrid

void
sendGrid( const pde::Grid& grid, int rank)
{
    auto [x_size, t_size] = grid.getSize();
    int data_size = x_size * t_size;

    MPI_Send(
        &rank, 1, MPI_INT,
        0, 0, 
        MPI_COMM_WORLD
    );

    MPI_Send(
        grid.getGrid(), data_size, MPI_DOUBLE,
        0, 0, 
        MPI_COMM_WORLD
    );
} // sendGrid

pde::Grid
collectGrid( pde::Grid& master_grid,
             int size)
{
    auto [x_size, t_size] = master_grid.getSize();
    auto [h, a] = master_grid.getPitch();

    pde::Grid grid{
        x_size * size, t_size, h, a,
        master_grid.getFunction(), master_grid.getLeftNeighbour()
    };

    const double* raw_grid = master_grid.getGrid();
    grid.copyGrid( raw_grid, x_size, t_size, 0, 0);

    std::vector<double> data{};
    int data_size = x_size * t_size;

    data.resize( data_size);

    for ( int i = 1; i < size; i++ )
    {
        int rank = 0;

        MPI_Recv(
            &rank, 1, MPI_INT,
            MPI_ANY_SOURCE, MPI_ANY_TAG, 
            MPI_COMM_WORLD, nullptr
        );

        MPI_Recv(
            data.data(), data_size, MPI_DOUBLE,
            rank, MPI_ANY_TAG,
            MPI_COMM_WORLD, nullptr
        );

        grid.copyGrid( data.data(), x_size, t_size, i * x_size, 0);
    }

    return grid;
} // collectGrid

void
runPDESchemeSeq( pde::Grid& grid,
                 pde::Scheme scheme,
                 std::string grid_name,
                 std::string scheme_name)
{
    const std::string kPlotDir = "assets/";
    visual::GNUPlot plot{};

    pde::solveGridSeq( grid, scheme);

    app::initPlot( plot, kPlotDir, "seq", grid_name, scheme_name);
    app::drawGrid( grid, plot);
    plot.makePlot();
} // runPDESchemeSeq

void
runPDESchemePar( pde::Grid& grid,
                 pde::Scheme scheme,
                 std::string grid_name,
                 std::string scheme_name)
{
    const std::string kPlotDir = "assets/";
    const int kMasterRank = 0;

    visual::GNUPlot plot{};

    int size = 0;
    int rank = 0;

    MPI_Comm_size( MPI_COMM_WORLD, &size);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);

    pde::solveGridPar( grid, scheme, rank);

    if ( rank == kMasterRank )
    {
        pde::Grid collection = collectGrid( grid, size);
        app::initPlot( plot, kPlotDir, "par", grid_name, scheme_name);
        app::drawGrid( collection, plot);
        plot.makePlot();
    } else
    {
        sendGrid( grid, rank);
    }
} // runPDESchemePar

void
runPDEAnalytical( const pde::Grid& grid,
                  pde::Grid::Function analytical,
                  std::string grid_name)
{
    const std::string kPlotDir = "assets/";
    visual::GNUPlot plot{};

    auto [x_size, t_size] = grid.getSize();
    auto [h, a] = grid.getPitch();

    pde::Grid analytical_grid{ x_size, t_size, h, a, nullptr};
    pde::Grid border_grid = createBorderGrid( analytical, t_size, a);
    analytical_grid.setLeftNeighbour( &border_grid);

    for ( int t = 0; t < t_size; t++ )
    {
        for ( int x = 0; x < x_size; x++ )
        {
            analytical_grid.setAbs( analytical( h*x, a*t), x, t);
        }
    }

    app::initPlot( plot, kPlotDir, "seq", grid_name, "analytical");
    app::drawGrid( analytical_grid, plot);
    plot.makePlot();
} // runPDEAnalytical

void
runPDEGridSeq( pde::Grid::Function fn,
               pde::Grid::Function analytical,
               std::string grid_name,
               int x_size,
               int t_size)
{
    pde::Grid grid = createGrid( fn, analytical, x_size, t_size, 0);
    pde::Grid border_grid = 
        createBorderGrid( analytical, grid.getSize().second, grid.getPitch().second);
    grid.setLeftNeighbour( &border_grid);

    runPDESchemeSeq( grid, pde::schemeCentral,   grid_name, "central");
    runPDESchemeSeq( grid, pde::schemeLax,       grid_name, "lax");
    runPDESchemeSeq( grid, pde::schemeRectangle, grid_name, "rectangle");
    runPDEAnalytical( grid, analytical, grid_name);
} // runPDEGridSeq

void
runPDEGridPar( pde::Grid::Function fn,
               pde::Grid::Function analytical,
               std::string grid_name,
               int x_size,
               int t_size)
{
    const int kMasterRank = 0;

    int size = 0;
    int rank = 0;

    MPI_Comm_size( MPI_COMM_WORLD, &size);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);

    pde::Grid grid = createGrid( fn, analytical, x_size / size, t_size, rank);
    pde::Grid neighbour_left{};
    pde::Grid neighbour_right{};

    /**
     * All processes have left neighbour.
     */
    if ( rank == kMasterRank )
    {
        neighbour_left = createBorderGrid( analytical, t_size, grid.getPitch().second);
    } else
    {
        neighbour_left = createNeighbourGrid( t_size);
    }
    grid.setLeftNeighbour( &neighbour_left);

    /**
     * Last (size - 1) process doesn't have right neighbour.
     */
    if ( rank < (size - 1) )
    {
        neighbour_right = createNeighbourGrid( t_size);
        grid.setRightNeighbour( &neighbour_right);
    }

    runPDESchemePar( grid, pde::schemeCentral,   grid_name, "central");
    runPDESchemePar( grid, pde::schemeLax,       grid_name, "lax");
} // runPDEGridPar

} // anonymous namespace

int
main( int argc, 
      char* argv[])
{
    const int x_size = 1000;
    const int t_size = 12000;

    MPI_Init( &argc, &argv);

    auto fn1 = []( double x, double t){ return 0.0; };
    auto fn2 = []( double x, double t){ return 2 * (x + t); };

    auto u1 = []( double x, double t){ return x - t; };
    auto u2 = []( double x, double t){ return x*x + t*t; };

    if ( argc == 1 )
    {
        runPDEGridSeq( fn1, u1, "(x-t)",     x_size, t_size);
        runPDEGridSeq( fn2, u2, "(x*x+t*t)", x_size, t_size);
    } else
    {
        double start = MPI_Wtime();
        runPDEGridPar( fn1, u1, "(x-t)",     x_size, t_size);
        runPDEGridPar( fn2, u2, "(x*x+t*t)", x_size, t_size);
        std::cout << MPI_Wtime() - start << std::endl;
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
} // main
