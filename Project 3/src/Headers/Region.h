#define REGION_DEF
#ifndef REGION_DEF

class Region {
    public:
    Region();
    void calculateIdealIndex();
    double getIdealIndex();
    
    private:
    double ideal_index;
}

#endif