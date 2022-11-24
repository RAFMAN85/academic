#ifndef SORTING_LAB1_SORT_CLASS_HPP
#define SORTING_LAB1_SORT_CLASS_HPP

#include <string>
#include <tuple>
#include <ostream>



    struct pers_information{
        int year_of_been;
        int squad;
        int team_number;
        std::string fio;


        pers_information(const std::string& fio,int year_of_been,int squad,int team_number);
        bool operator <(const pers_information& character) const;
        bool operator >(const pers_information& character) const;
        bool operator <=(const pers_information& character) const;
        bool operator >=(const pers_information& character) const;
        bool operator ==(const pers_information& character) const;
        bool operator !=(const pers_information& character) const;
        friend std::ostream& operator <<(std::ostream &out,const pers_information& character);




    };


#endif //SORTING_LAB1_SORT_CLASS_HPP
