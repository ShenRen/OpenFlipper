#pragma once

#include <ObjectTypes/PolyLine/PolyLineT.hh>

#include <queue>

template <typename T>
class PolyLineCollectionT{
public:
    typedef typename ACG::PolyLineT<T> PolyLine;

    struct iterator{
        iterator(typename std::vector<PolyLine*>::iterator _begin, typename std::vector<PolyLine*>::iterator _end) :
            i_(0),
            it_(_begin),
            end_(_end)
        { }

        PolyLine*& operator* (){
            return *it_;
        }

        PolyLine* operator-> (){
            return *it_;
        }

        void operator++(){
            ++i_;
            ++it_;
        }

        int idx(){
            return i_;
        }

        operator bool() const{
            return it_ != end_;
        }

        int i_;
        typename std::vector<PolyLine*>::iterator it_;
        typename std::vector<PolyLine*>::iterator end_;
    };

    struct index_iterator{
        index_iterator(std::vector<PolyLine*>& _lines, typename std::vector<int>::iterator _begin, typename std::vector<int>::iterator _end) :
            lines_(_lines),
            it_(_begin),
            end_(_end)
        { }

        PolyLine*& operator* (){
            return lines_[*it_];
        }

        PolyLine* operator-> (){
            return lines_[*it_];
        }

        void operator++(){
            ++it_;
        }

        int idx(){
            return *it_;
        }

        operator bool() const{
            return it_ != end_;
        }

        std::vector<PolyLine*>& lines_;
        typename std::vector<int>::iterator it_;
        typename std::vector<int>::iterator end_;
    };

public:


    virtual ~PolyLineCollectionT(){
        int n = poly_lines_.size();
        for(int i = 0; i < n; ++i){
            delete poly_lines_[i];
        }
    }

    // Create a new polyline in the collection
    int add_poly_line(PolyLine* _poly_line){

        int new_idx;
        if(empty_slots_.empty()){
            new_idx = poly_lines_.size();
            poly_lines_.push_back(0);
        }else{
            new_idx = empty_slots_.front();
            empty_slots_.pop();
        }

        poly_lines_[new_idx] = _poly_line;
        visible_.push_back(new_idx);

        return new_idx;
    }

    int new_poly_line(){
        PolyLine* pl = new PolyLine();
        pl->request_vertex_selections();
        pl->request_edge_selections();
        pl->request_vertex_vhandles();
        pl->request_vertex_ehandles();
        pl->request_vertex_fhandles();
        pl->request_edge_scalars();

        return add_poly_line(pl);
    }

    // Reserve space for additional _count polylines
    void reserve(int _count){
        poly_lines_.reserve(poly_lines_.size() + _count);
    }

    // Accesssors
    inline int n_polylines(){return poly_lines_.size();}
    inline PolyLine* polyline(int _i){return poly_lines_[_i];}

    inline int n_visible_polylines(){return visible_.size();}
    inline PolyLine* visible_polyline(int _i){return poly_lines_[visible_[_i]];}

    // Iterators
    inline iterator iter(){return iterator(poly_lines_.begin(), poly_lines_.end());}
    inline index_iterator visible_iter(){return index_iterator(poly_lines_, visible_.begin(), visible_.end());}
    inline index_iterator selected_iter(){return index_iterator(poly_lines_, selected_.begin(), selected_.end());}

    void set_visible_all()
    {
        int n_lines = poly_lines_.size();
        visible_.resize(n_lines);
        for(int i = 0; i < n_lines; ++i){
            visible_[i] = i;
        }
    }
    void set_visible(const std::vector<int>& _visible)
    {
        visible_ = _visible;
    }

    void set_selected(const std::vector<int>& _selected)
    {
        selected_ = _selected;
    }

    void clear_selection(){
        selected_.clear();
    }

    void clear(){
        for (size_t i = 0; i < poly_lines_.size(); ++i)
        {
            delete poly_lines_[i];
        }
        poly_lines_.clear();
        visible_.clear();
        while(!empty_slots_.empty()) empty_slots_.pop();
    }

    void remove_polyline(int _id){
        if(_id < int(poly_lines_.size()) && poly_lines_[_id] != 0){
            delete poly_lines_[_id];
            poly_lines_[_id] = 0;
            empty_slots_.push(_id);
        }
    }


    /*
    Ultimaker plugin color settings:

    color_map_[Ultimaker::GC_WALL_OUTER] = ACG::Vec4uc(0,0,255,255);
    color_map_[Ultimaker::GC_INFILL] = ACG::Vec4uc(0,120,120,255);
    color_map_[Ultimaker::GC_MOVE] = ACG::Vec4uc(0, 255, 0,255);
    color_map_[Ultimaker::GC_WALL_INNER] = ACG::Vec4uc(0, 255,255,255);
    color_map_[Ultimaker::GC_TOPBOTTOM] = ACG::Vec4uc(0,180,180,255);
    color_map_[Ultimaker::GC_SUPPORT_ACC] = ACG::Vec4uc(180,20,20,255);
    color_map_[Ultimaker::GC_SUPPORT_ACC_LE] = ACG::Vec4uc(200,140,15,255);
    color_map_[Ultimaker::GC_SUPPORT] = ACG::Vec4uc(255,100,66,255);
    color_map_[Ultimaker::GC_BRIM] = ACG::Vec4uc(255,210,0,255);
    */

    inline void set_color(int _edge_scalar, const ACG::Vec4uc& _color){color_map_[_edge_scalar] = _color;}
    inline ACG::Vec4uc color(int _edge_scalar) {return color_map_[_edge_scalar];}

protected:
    std::vector<PolyLine*> poly_lines_;
    std::vector<int> visible_;
    std::vector<int> selected_;

    std::queue<int> empty_slots_;

    // Color map for edge scalars
    std::map<int, ACG::Vec4uc> color_map_;
};
