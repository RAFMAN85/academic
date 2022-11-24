#include "sort_class.hpp"
#include <tuple>

pers_information::pers_information(const std::string& fio,int year_of_been,int squad,int team_number)
{
    this->fio=fio;
    this->year_of_been=year_of_been;
    this->squad=squad;
    this->team_number=team_number;

}

bool pers_information::operator <(const pers_information& character) const
{
    return std::tie(fio,year_of_been,squad,team_number)<std::tie(character.fio,character.year_of_been,character.squad,character.team_number);
}

bool pers_information::operator >(const pers_information& character) const
{
    return std::tie(fio,year_of_been,squad,team_number)>std::tie(character.fio,character.year_of_been,character.squad,character.team_number);
}
bool pers_information::operator <=(const pers_information& character) const
{
    return !(*this>character);
}

bool pers_information::operator >=(const pers_information& character) const
{
    return !(*this<character);
}

bool pers_information::operator ==(const pers_information& character) const
{
    return year_of_been==character.year_of_been && squad==character.squad && team_number==character.team_number && fio==character.fio;
}
bool pers_information::operator !=(const pers_information& character) const
{
    return !(*this==character);
}
std::ostream& operator <<(std::ostream &out,const pers_information& character)
{
        out << character.year_of_been << " " << character.squad << " " << character.team_number << " " << character.fio;
}

