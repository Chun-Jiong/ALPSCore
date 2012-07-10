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

#ifndef ALPS_NUMERIC_MATRIX_TRANSPOSE_HPP
#define ALPS_NUMERIC_MATRIX_TRANSPOSE_HPP
#include <alps/numeric/matrix/transpose_view.hpp>

namespace alps {
namespace numeric {

template <typename Matrix>
inline transpose_view<Matrix> transpose(Matrix& m) {
    return transpose_view<Matrix>(m);
}

template <typename Matrix>
inline transpose_view<Matrix const> transpose(Matrix const& m) {
    return transpose_view<Matrix const>(m);
}

template <typename Matrix>
void transpose_inplace(Matrix& m) {
    typedef typename Matrix::size_type size_type;
    using std::swap;
    if(num_rows(m) == num_cols(m) ) {
        for(size_type i = 0; i < num_rows(m); ++i)
            for(size_type j = i+1; j < num_cols(m); ++j)
                swap(m(i,j),m(j,i));
    } else {
        // TODO replace this code by an actual inplace implementation
        Matrix m2 = transpose(m);
        swap(m,m2);
    }
}

} // end namespace numeric
} // end namespace alps
#endif //ALPS_NUMERIC_MATRIX_TRANSPOSE_HPP
