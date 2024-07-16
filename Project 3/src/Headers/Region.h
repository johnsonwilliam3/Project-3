#ifndef REGION_H
#define REGION_H
class Region {
    public:
    Region();
    void calculateIdealIndex();
    double getIdealIndex();
    
    private:
    double ideal_index;
};

#endif