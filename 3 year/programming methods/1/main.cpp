#include <iostream>
#include <vector>
#include <cstddef>
#include <utility>
#include "sort_class.hpp"
#include <fstream>
#include <sstream>
#include <tuple>
#include <ctime>
#include <map>


void print_map(std::multimap <std::string,pers_information> m)
{
    for (auto [first,second] : m)
    {
        std::cout <<first<<":"<< second<<"\n";
    }
}


//print base vector
template<typename T>
void print_vector(std::vector<T>& vect)
{
    for (auto val : vect)
    {
        std::cout << val <<'\n';
    }
}



//printing personal vector from class
template<typename T>
void print_vector_pers(std::vector<T>& vect)
{
    for (auto val : vect)
    {
        std::cout << val<<'\n';
    }
}



// selection sorting (a)
    template<typename T>
    void select_sort(std::vector<T> &mass) {
        for (size_t ind_i = 0; ind_i < mass.size() - 1; ind_i++) {
            int min_ind = ind_i;
            for (size_t ind_j = ind_i + 1; ind_j < mass.size(); ind_j++) {
                if (mass[ind_j] < mass[min_ind]) {
                    min_ind = ind_j;
                }
            }
            if (min_ind != ind_i) {
            std::swap(mass[ind_i], mass[min_ind]);
            }
        }
    }

//simple insertion sorting (v)
    template<typename T>
    void insert_sort(std::vector<T> &mass1) {
        for (int i = 1; i < mass1.size(); i++)
            for (int j = i; j > 0 && mass1[j - 1] > mass1[j]; j--)
                std::swap(mass1[j - 1], mass1[j]);
    }

//Shaker sorting (g)
    template<typename T>
    void shaker_sort(std::vector<T> &mass2) {
        int left, right;
        left = 1;
        right = mass2.size() - 1;
        while (left <= right) {
            for (int i = left; i <= right; i++) {
                if (mass2[i - 1] > mass2[i]) {
                    std::swap(mass2[i - 1], mass2[i]);
                }
            }
            right--;
            for (int i = right; i >= left; i--) {
                if (mass2[i - 1] > mass2[i]) {
                    std::swap(mass2[i - 1], mass2[i]);
                }
            }
            left++;
        }
    }


//straight search
    std::vector<pers_information> straight_s(std::vector<pers_information> vect1, std::string key) {
        std::vector<pers_information> buffer_vect1;
        for (int i = 0; i < vect1.size(); i++) {
            if (key == vect1[i].fio) {
                buffer_vect1.push_back(vect1[i]);
            }
        }
        return buffer_vect1;
    }


//binary search
    std::vector<pers_information> binary_s(std::vector<pers_information> vect2, std::string key) {
        std::vector<pers_information> buffer_vect2;
        int left = 0;
        int right = vect2.size();
        int middle=0;

        while (left < right) {
            middle = (left + right) / 2;
            if (key < vect2[middle].fio) {
                right = middle - 1;
            } else if (key > vect2[middle].fio) {
                left = middle + 1;
            } else {
                left = right;
            }

        }
        buffer_vect2.push_back(vect2[middle]);
        int ind_left = middle - 1;
        int ind_right = middle + 1;
        while (vect2[middle].fio == vect2[ind_right].fio && ind_right <= vect2.size()) {
            buffer_vect2.push_back(vect2[ind_right]);
            ind_right++;
        }
        while (vect2[middle].fio == vect2[ind_left].fio && ind_left >= 0) {
            buffer_vect2.push_back(vect2[ind_left]);
            ind_left--;
        }
        return buffer_vect2;
    }




    int main() {
    std::vector<pers_information> children;
    std::string buf_working;
    std::ifstream in("/home/rafman/Desktop/100000.txt");
    while (std::getline(in, buf_working))
    {
        std::istringstream for_search(buf_working);
        std::string word;
        std::vector<std::string> words;
        while (getline(for_search, word, ';'))
        {
            words.push_back(word);
        }
        pers_information child(words[2], std::stoi(words[0]), std::stoi(words[1]), std::stoi(words[4]));
        children.push_back(child);
    }
        std::multimap<std::string,pers_information> test_map;

        for(int i=0;i<children.size();i++)
        {
            test_map.emplace(children[i].fio,children[i]);
        }
        std::vector<pers_information> children_bin_s=children;
        std::vector<pers_information> children_str=children;
        std::vector<pers_information> children_bin=children;



        insert_sort(children_bin_s);
        clock_t bin_s_sort_start=clock();
        //binary_search
        std::vector<pers_information> for_out_bin = binary_s(children_bin_s, "Чубайс Валерий Олегович");
        clock_t bin_s_sort_end=clock();
        double bin_s_sort_time = (double)(bin_s_sort_end-bin_s_sort_start)/CLOCKS_PER_SEC;
        std::cout<<"binary search with sort: "<<bin_s_sort_time<<'\n';


        clock_t stra_s_start=clock();
        //straight_search
        std::vector<pers_information> for_out_straight=straight_s(children_str,"Чубайс Валерий Олегович");
        clock_t stra_s_end=clock();
        double stra_s_time = (double)(stra_s_end-stra_s_start)/CLOCKS_PER_SEC;
        std::cout<<"straight search: "<<stra_s_time<<'\n';

        clock_t bin_s_start=clock();
        std::vector<pers_information> for_out_bin_no_sort=binary_s(children_bin,"Чубайс Валерий Олегович");
        clock_t bin_s_end=clock();
        double bin_s_time = (double)(bin_s_end-bin_s_start)/CLOCKS_PER_SEC;
        std::cout<<"binary search: "<<bin_s_time<<'\n';


        clock_t map_start=clock();
        //multimap
        auto p = test_map.equal_range("Чубайс Валерий Олегович");
        clock_t map_end=clock();
        double map_time = (double)(map_end-map_start)/CLOCKS_PER_SEC;
        std::cout<<"search with multimap: "<<map_time<<'\n';



        return 0;
    }
