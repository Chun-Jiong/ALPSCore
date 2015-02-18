/*
 * Copyright (C) 1998-2014 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include <alps/utilities/signal.hpp>
#include <alps/mc/mcbase.hpp>

namespace alps {

    mcbase::mcbase(parameters_type const & parms, std::size_t seed_offset)
        : parameters(parms)
          // , params(parameters) // TODO: remove, deprecated!
        , random(std::size_t(parameters["SEED"]) + seed_offset)
    {
        alps::signal::listen();
    }

    void mcbase::define_parameters(parameters_type & parameters) {
        parameters
            .define("continue", "load simulation from checkpoint")
            .define<std::size_t>("SEED", 42, "PRNG seed")
            .define<std::size_t>("timelimit", 0, "time limit for the simulation")
            // .define<std::size_t>("Tmin", 600, "maximum time to check if simulation has finished")
            // .define<std::size_t>("Tmax", "number of sweeps for thermalization")
            .define<std::string>("outputfile", "name of the output file")
        ;
    }

    void mcbase::save(boost::filesystem::path const & filename) const {
        alps::hdf5::archive ar(filename, "w");
        ar["/simulation/realizations/0/clones/0"] << *this;
    }

    void mcbase::load(boost::filesystem::path const & filename) {
        alps::hdf5::archive ar(filename);
        ar["/simulation/realizations/0/clones/0"] >> *this;
    }

    bool mcbase::run(boost::function<bool ()> const & stop_callback) {
        bool stopped = false;
        while(!(stopped = stop_callback()) && fraction_completed() < 1.) {
            update();
            measure();
        }
        return !stopped;
    }

    // implement a nice keys(m) function
    mcbase::result_names_type mcbase::result_names() const {
        result_names_type names;
        for(observable_collection_type::const_iterator it = measurements.begin(); it != measurements.end(); ++it)
            names.push_back(it->first);
        return names;
    }

    mcbase::result_names_type mcbase::unsaved_result_names() const {
        return result_names_type(); 
    }

    mcbase::results_type mcbase::collect_results() const {
        return collect_results(result_names());
    }

    mcbase::results_type mcbase::collect_results(result_names_type const & names) const {
        results_type partial_results;
        for(result_names_type::const_iterator it = names.begin(); it != names.end(); ++it){
                partial_results.insert(*it, measurements[*it].result());
        }
        return partial_results;
    }

    void mcbase::save(alps::hdf5::archive & ar) const {
        ar["/parameters"] << parameters;
        ar["measurements"] << measurements;
        ar["checkpoint"] << random;
    }

    void mcbase::load(alps::hdf5::archive & ar) {
        ar["/parameters"] >> parameters;
        ar["measurements"] >> measurements;
        ar["checkpoint"] >> random;
    }

}
