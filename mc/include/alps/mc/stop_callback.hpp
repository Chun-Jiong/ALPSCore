/*
 * Copyright (C) 1998-2017 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_CALLBACK_HPP
#define ALPS_CALLBACK_HPP

#include <alps/config.hpp>
#include <alps/utilities/signal.hpp>

#include <boost/chrono.hpp>
#ifdef ALPS_HAVE_MPI
#include <boost/optional.hpp>
# include "alps/utilities/mpi.hpp"
#endif

namespace alps {

	class stop_callback {
		public:
		    stop_callback(std::size_t timelimit);
#ifdef ALPS_HAVE_MPI
			stop_callback(alps::mpi::communicator const & cm, std::size_t timelimit);
#endif
		    bool operator()() const;
		private:
		    const boost::chrono::duration<std::size_t> limit;
		    alps::signal signals;
		    const boost::chrono::high_resolution_clock::time_point start;
#ifdef ALPS_HAVE_MPI
	        boost::optional<alps::mpi::communicator> comm;
#endif
	};

	class simple_time_callback {
		public:
		    simple_time_callback(std::size_t timelimit);
		    bool operator()() const;
		private:
		    const boost::chrono::duration<std::size_t> limit;
		    const boost::chrono::steady_clock::time_point start;
	};
}



#endif
