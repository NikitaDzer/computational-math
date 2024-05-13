#pragma once

#include <cassert>
#include <functional>
#include <memory>

namespace pde
{

class Grid
{

public:
    using Function = std::function<double( double x, double t)>;

public:
    Grid() = default;

    Grid( int x_size, int t_size,
          double h, double a,
          Grid::Function function,
          Grid* neighbour_left = nullptr,
          Grid* neighbour_right = nullptr):
        m_x_size{ x_size}, m_t_size{ t_size},
        m_x_cur{ 0}, m_t_cur{ 0},
        m_h{ h}, m_a{ a},
        m_function{ function}, 
        m_neighbour_left{ neighbour_left},
        m_neighbour_right{ neighbour_right},
        m_grid{ nullptr}
    {
        resize( x_size, t_size);
    }

    Grid( Grid&& grid) = default;
    Grid( const Grid& grid):
        m_x_size{ grid.m_x_size}, m_t_size{ grid.m_t_size},
        m_x_cur{ grid.m_x_cur}, m_t_cur{ grid.m_t_cur},
        m_h{ grid.m_h}, m_a{ grid.m_a},
        m_function{ grid.m_function}, 
        m_neighbour_left{ grid.m_neighbour_left},
        m_neighbour_right{ grid.m_neighbour_right},
        m_grid{ nullptr}
    {
        resize( m_x_size, m_t_size);
        copyGrid( grid.getGrid(), m_x_size, m_t_size, 0, 0);
    }

    Grid& operator = ( Grid&& grid) = default;
    Grid& operator = ( const Grid& grid)
    {
        Grid tmp{ grid};
        std::swap( *this, tmp);

        return *this;
    }

    double get( int x = 0, int t = 0) const
        { return accessNode( m_x_cur + x, m_t_cur + t); }
    void set( double value, int x = 0, int t = 0)
        { accessNode( m_x_cur + x, m_t_cur + t) = value; }

    double getAbs( int x, int t) const
        { return accessNode( x, t); }
    void setAbs( double value, int x, int t)
        { accessNode( x, t) = value; }

    double getLeft( int t) const
        { return getAbs( 0, t); }
    void setLeft( double value, int t)
        { setAbs( value, 0, t); }
    double getRight( int t) const
        { return getAbs( m_x_size - 1, t); }
    void setRight( double value, int t)
        { setAbs( value, m_x_size - 1, t); }
    
    std::pair<int, int> getCurrentNode() const
        { return {m_x_cur, m_t_cur}; }
    void setCurrentNode( int x, int t)
        { m_x_cur = x; m_t_cur = t; }
    void moveCurrentNode( int step = 1)
    {
        int x_new = (m_x_cur + step) % m_x_size;
        int t_new = m_t_cur + (m_x_cur + step) / m_x_size;

        setCurrentNode( x_new, t_new);
    }
    bool isCurrentNodeLeft() const
        { return m_x_cur == 0; }
    bool isCurrentNodeRight() const
        { return m_x_cur == (m_x_size - 1); }

    std::pair<int, int> getSize() const
        { return {m_x_size, m_t_size}; }

    std::pair<double, double> getPitch() const
        { return {m_h, m_a}; }

    Grid::Function getFunction() const
        { return m_function; }

    Grid* getLeftNeighbour() const
        { return m_neighbour_left; }
    void setLeftNeighbour( Grid* neighbour_left)
    { 
        m_neighbour_left = neighbour_left;
        neighbour_left->m_neighbour_right = this;
    }
    bool hasLeftNeighbour() const
        { return m_neighbour_left != nullptr; }

    Grid* getRightNeighbour() const
        { return m_neighbour_right; }
    void setRightNeighbour( Grid* neighbour_right)
    { 
        m_neighbour_right = neighbour_right;
        neighbour_right->m_neighbour_left = this;
    }
    bool hasRightNeighbour() const
        { return m_neighbour_right != nullptr; }

    void resize( int x_size, int t_size)
        { m_grid = std::make_unique<double[]>( x_size * t_size); }

    const double* getGrid() const
        { return m_grid.get(); }

    void copyGrid( const double* raw_grid, 
                   int x_size, int t_size,
                   int x_shift, int t_shift)
    {
        for ( int t = 0; t < t_size; t++ )
        {
            for ( int x = 0; x < x_size; x++ )
            {
                setAbs( raw_grid[x + t * x_size], x_shift + x, t_shift + t);
            }
        }
    }


private:
    int m_x_size;
    int m_t_size;

    int m_x_cur;
    int m_t_cur;

    double m_h;
    double m_a;

    Grid::Function m_function;

    Grid* m_neighbour_left;
    Grid* m_neighbour_right;

    std::unique_ptr<double[]> m_grid;


private:
    double& accessNode( int x, int t) const
    {
        assert( 0 <= x && x < m_x_size);
        assert( 0 <= t && t < m_t_size);

        return m_grid[(t * m_x_size) + x];
    }

}; // class Grid

const inline Grid kDefaultGrid{
    1, 1, 0, 0, nullptr
};

} // namespace pde
