/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef LOGGER_H__
#define LOGGER_H__

#include <boost/log/trivial.hpp>

using namespace boost::log::trivial;
#define __SHORT_FILE2__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define __SHORT_FILE__ (strrchr(__SHORT_FILE2__, '\\') ? strrchr(__SHORT_FILE2__, '\\') + 1 : __SHORT_FILE2__)
#define LOG(level) BOOST_LOG_TRIVIAL(level) << __SHORT_FILE__ << ":" << __LINE__ << " "

#endif // LOGGER_H__
