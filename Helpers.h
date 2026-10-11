#pragma once
#ifndef FSH_HELPERS_H
#define FSH_HELPERS_H

#include <iostream>
#include <sys/stat.h>
#include <cstdint>
#include <ctime>
#include <sys/timeb.h>
#include <algorithm>
#include <sstream>
#include <string>
#include <cstring>
#include <initializer_list>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <regex>
#include <thread>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string.hpp>

#include <random>

#define DELIM "."

class Helpers {
private:
public:

    std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
    {
        str.erase(0, str.find_first_not_of(chars));
        return str;
    }

    std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
    {
        str.erase(str.find_last_not_of(chars) + 1);
        return str;
    }

    std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
    {
        return ltrim(rtrim(str, chars), chars);
    }

    bool ReplaceWithString(std::string &str, std::string oldstr, std::string newstr)
    {
        size_t start_pos = str.find(oldstr);
        if(start_pos == std::string::npos)
            return false;
        str.replace(start_pos, oldstr.length(), newstr);

        size_t start_pos_s = str.find(oldstr);
        if(start_pos_s != std::string::npos)
            return ReplaceWithString(str, oldstr, newstr);
        return true;
    }

    std::string RemoveAllChar(std::string str, std::string oldstr)
    {
        boost::erase_all(str, oldstr);
        return str;
    }

    void Println(int t)
    {
        std::cout << t <<std::endl;
    }

    void Println(char t)
    {
        std::cout << t <<std::endl ;
    }

    void Println(std::string t)
    {
        std::cout << t <<std::endl ;
    }

    void Print(int t)
    {
        std::cout << t;
    }

    void Print(char t)
    {
        std::cout << t;
    }

    void Print(std::string t)
    {
        std::cout << t;
    }

};


#endif //FCRPT_HELPERS_H
