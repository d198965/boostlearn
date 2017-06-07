//
// Created by zdh on 17/6/7.
//

#ifndef UNTITLED1_PROTLCAL_H_H
#define UNTITLED1_PROTLCAL_H_H

#endif //UNTITLED1_PROTLCAL_H_H
#pragma once

#include <boost/serialization/access.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

struct message_t
{
    int num;
    std::string str;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int /*version*/)
    {
        ar & num;
        ar & str;
    }
};