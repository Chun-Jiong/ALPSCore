/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                                 *
 * ALPS Project: Algorithms and Libraries for Physics Simulations                  *
 *                                                                                 *
 * ALPS Libraries                                                                  *
 *                                                                                 *
 * Copyright (C) 2010 - 2011 by Lukas Gamper <gamperl@gmail.com>                   *
 *                           Matthias Troyer <troyer@comp-phys.org>                *
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

#include <alps/ngs/mcoptions.hpp>

#include <boost/program_options.hpp>

#include <iostream>

namespace alps {

    mcoptions::mcoptions(int argc, char* argv[]) : valid(false), resume(false), type(SINGLE) {
        boost::program_options::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("single", "run single process")
            #ifndef ALPS_NGS_SINGLE_THREAD
                ("threaded", "run in multithread envirement")
            #endif
            #ifdef ALPS_HAVE_MPI
                ("mpi", "run in parallel using MPI")
            #endif
            ("continue", "load simulation from checkpoint")
            ("time-limit,T", boost::program_options::value<std::size_t>(&time_limit)->default_value(0), "time limit for the simulation")
            ("input-file", boost::program_options::value<std::string>(&input_file), "input file in hdf5 format")
            ("output-file", boost::program_options::value<std::string>(&output_file)->default_value("sim.h5"), "output file in hdf5 format");
        boost::program_options::positional_options_description p;
        p.add("input-file", 1);
        p.add("output-file", 2);
        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        boost::program_options::notify(vm);
        if (!(valid = !vm.count("help")))
            std::cout << desc << std::endl;
        else if (input_file.empty())
            throw std::invalid_argument("No job file specified");
        if (vm.count("threaded") && vm.count("mpi"))
            type = HYBRID;
        else if (vm.count("threaded"))
            #ifdef ALPS_NGS_SINGLE_THREAD
                throw std::logic_error("Not build with multithread support");
            #else
                type = THREADED;
            #endif
        else if (vm.count("mpi")) {
            type = MPI;
            #ifndef ALPS_HAVE_MPI
                throw std::logic_error("Not build with MPI");
            #endif
        }
        if (vm.count("continue"))
            resume = true;
    }

}