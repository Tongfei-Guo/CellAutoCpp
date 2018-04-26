#ifndef CAMEASURE_H
#define CAMEASURE_H

#include "Cell.h"
#include "CATypes.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <map>

class CAMeasure{

public:
    CAMeasure(const std::string &_name):name(_name){}
    virtual void Init()=0;
    virtual void NewRecord()=0;
    virtual void Update(Cell *cell)=0;
    virtual std::string Str_Current()=0;
    virtual std::string Str_All()=0;
    std::string GetName(){ return name; }

protected:
    std::string name;
};

class CADistributionMeasure : public CAMeasure{

    typedef std::unordered_map<type_name, std::unordered_map<state_name, std::unordered_map<state_value, int> > > stats_frame_type;

public:
    CADistributionMeasure(std::string _name="State Value Distribution"):CAMeasure(_name){
        Init();
    }

    void Init(){
        stats.clear();
    }

    void NewRecord(){
        stats.push_back(stats_frame_type());
    }

    void Update(Cell *cell){
        for(const auto &s: cell->get_states())
            stats.back()[cell->get_type()][s.first][s.second]++;
    }

    std::string Str_Current(){
        if (stats.size()>0)
            return Str_Index(stats.size()-1);
        else
            return "NULL";
    }

    std::string Str_All(){
        std::string ret;
        for(int i=0; i<stats.size(); i++)
            ret+=std::to_string(i)+":\n"+Str_Index(i,"\t")+"\n";
        return ret;
    }

private:
    std::vector<stats_frame_type> stats;

    std::string Str_Index(int i, std::string prefix=""){
        if (i<0 || i>=stats.size())
            return std::string("Failure to index ")+std::to_string(i);
        std::string ret;
        for(const auto &a: stats[i]){
            ret+=prefix+std::string("Cell Type = ")+(std::string)a.first+std::string(":\n");
            for(const auto &b: a.second){
                ret+=prefix+std::string("\t");
                ret+=std::string("State Name = ")+(std::string)b.first+std::string(" : ");
                bool first=true;
                std::map<state_value,int> out_map;
                for(const auto &c: b.second)
                    out_map[c.first]=c.second;
                for(const auto &c: out_map){
                    if (!first)
                        ret+=", ";
                    ret+=std::string("(\"")+std::to_string((int)c.first)+std::string("\"=")+std::to_string((int)c.second)+std::string(")");
                    first=false;
                }
                ret+=std::string("\n");
            }
            ret+=std::string("\n");
        }
        return ret;
    }

};

#endif
