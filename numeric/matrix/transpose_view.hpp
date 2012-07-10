/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                                 *
 * ALPS Project: Algorithms and Libraries for Physics Simulations                  *
 *                                                                                 *
 * ALPS Libraries                                                                  *
 *                                                                                 *
 * Copyright (C) 2012 by Andreas Hehn <hehn@phys.ethz.ch>                          *
 *                                                                                 *
 * This software is part of the ALPS libraries, published under the ALPS           *
 * Library License; you can use, redistribute it and/or modify it under            *
 * the terms of the license, either version 1 or (at your option) any later        *
 * version.                                                                        *
 *                                                                                 *
 * You should have received a copy of the ALPS Library License along with          *
 * the ALPS Libraries; see the file LICENSE.txt. If not, the license is also       *
 * available from http://alps.comp-phys.org/.                                      *
 *                                                                                 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT       *
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE       *
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,     *
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER     *
 * DEALINGS IN THE SOFTWARE.                                                       *
 *                                                                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ALPS_MATRIX_TRANSPOSE_VIEW_HPP
#define ALPS_MATRIX_TRANSPOSE_VIEW_HPP

#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <alps/numeric/matrix/detail/transpose_view_adaptor.hpp>

namespace alps {
namespace numeric {

template <typename Matrix>
class transpose_view;


template <typename Matrix>
struct transpose_helper {
};

template <typename T, typename MemoryBlock>
struct transpose_helper<matrix<T,MemoryBlock> const> {
    static matrix<T,MemoryBlock> apply( transpose_view<matrix<T,MemoryBlock> const> const& v) {
        typedef typename transpose_view<matrix<T,MemoryBlock> const>::const_col_element_iterator const_col_element_iterator;
        std::vector<std::pair<const_col_element_iterator,const_col_element_iterator> > columns;
        for(std::size_t i=0; i < num_cols(v); ++i)
            columns.push_back(col(v,i));
        return matrix<T,MemoryBlock>(columns);
    }
};

template <typename Matrix>
class transpose_view {
  public:
    // typedefs required for a std::container concept
    typedef typename Matrix::value_type         value_type;       // The type T of the elements of the matrix
    typedef typename Matrix::reference          reference;        // Reference to value_type
    typedef typename Matrix::const_reference    const_reference;  // Const reference to value_type
    typedef typename Matrix::size_type          size_type;        // Unsigned integer type that represents the dimensions of the matrix
    typedef typename Matrix::difference_type    difference_type;  // Signed integer type to represent the distance of two elements in the memory

    // for compliance with an std::container one would also need
    // -operators == != < > <= >=
    // -size()
    // -typedefs iterator, const_iterator

    // typedefs for matrix specific iterators
    typedef typename Matrix::col_element_iterator
        row_element_iterator;                         // Iterator to iterate through the elements of a row of the matrix
    typedef typename Matrix::const_col_element_iterator
        const_row_element_iterator;                   // Const version of row_element_iterator
    typedef typename Matrix::row_element_iterator
        col_element_iterator;                         // Iterator to iterate through the elements of a columns of the matrix
    typedef typename Matrix::const_row_element_iterator
        const_col_element_iterator;                   // Const version of col_element_iterator
//    typedef matrix_element_iterator<matrix,value_type>
//        element_iterator;                             // Iterator to iterate through all elements of the matrix (REALLY SLOW! USE row_-/column_iterators INSTEAD!)
//    typedef matrix_element_iterator<const matrix,const value_type>
//        const_element_iterator;                       // Const version of element_iterator (REALLY SLOW! USE row_-/column_iterators INSTEAD!)

    explicit transpose_view(Matrix& m)
    : m_(m){
    };

    operator Matrix() const {
        return transpose_helper<typename boost::add_const<Matrix>::type>::apply(transpose_view<typename boost::add_const<Matrix>::type>(const_cast<Matrix const&>(m_)));
    }

    inline value_type& operator()(size_type i, size_type j) {
        return m_(j,i);
    }

    inline value_type const& operator()(size_type i, size_type j) const {
        return m_(j,i);
    }

    inline size_type num_rows() const {
        return m_.num_cols();
    }

    inline size_type num_cols() const {
        return m_.num_rows();
    }

    inline difference_type stride1() const {
        return m_.stride2();
    }

    inline difference_type stride2() const {
        return m_.stride1();
    }

    inline std::pair<row_element_iterator,row_element_iterator> row(size_type row = 0)
    {
        return m_.col(row);
    }

    inline std::pair<const_row_element_iterator,const_row_element_iterator> row(size_type row = 0) const
    {
        return m_.col(row);
    }
    inline std::pair<col_element_iterator,col_element_iterator> col(size_type col = 0 )
    {
        return m_.row(col);
    }
    inline std::pair<const_col_element_iterator,const_col_element_iterator> col(size_type col = 0 ) const
    {
        return m_.row(col);
    }

  private:
    Matrix& m_;
};


template <typename Matrix1, typename Matrix2>
typename matrix_matrix_multiply_return_type<Matrix1, transpose_view<Matrix2> >::type operator * (Matrix1 const& m1, transpose_view<Matrix2> const& m2) {
    return matrix_matrix_multiply(m1,m2);
}
template <typename Matrix1, typename Matrix2>
typename matrix_matrix_multiply_return_type<transpose_view<Matrix1>, Matrix2 >::type operator * (transpose_view<Matrix1> const& m1, Matrix2 const& m2) {
    return matrix_matrix_multiply(m1,m2);
}

template <typename Matrix1, typename Matrix2>
typename matrix_matrix_multiply_return_type<transpose_view<Matrix1>, transpose_view<Matrix2> >::type operator * (transpose_view<Matrix1> const& m1, transpose_view<Matrix2> const& m2) {
    return matrix_matrix_multiply(m1,m2);
}

//
// Implement the default matrix interface
//

ALPS_IMPLEMENT_MATRIX_INTERFACE(transpose_view<Matrix>,<typename Matrix>)

//
// Specializations
//

template <typename Matrix1, typename Matrix2>
struct matrix_matrix_multiply_return_type<Matrix1, transpose_view<Matrix2> > {
    typedef typename matrix_matrix_multiply_return_type<
          typename boost::remove_const<Matrix1>::type
        , typename boost::remove_const<Matrix2>::type
    >::type type;
};

template <typename Matrix1, typename Matrix2>
struct matrix_matrix_multiply_return_type<transpose_view<Matrix1>, Matrix2> {
    typedef typename matrix_matrix_multiply_return_type<
          typename boost::remove_const<Matrix1>::type
        , typename boost::remove_const<Matrix2>::type
    >::type type;
};

template <typename Matrix>
struct matrix_matrix_multiply_return_type<transpose_view<Matrix>, transpose_view<Matrix> > {
    typedef Matrix type;
};
template <typename Matrix1, typename Matrix2>
struct is_matrix_scalar_multiplication<Matrix1,transpose_view<Matrix2> > {
    static bool const value = false;
};

} // end namespace numeric
} // end namespace alps

#endif // ALPS_MATRIX_TRANSPOSE_VIEW_HPP

