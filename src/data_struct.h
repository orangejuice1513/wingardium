// defines the 2 state structures for drone movement 

// raw state 
struct raw_state = {
    bool enable_control, 
    float xin, 
    float yin, 
    float ax, 
    float ay, 
    float az, 
    float gx, 
    float gy, 
    float gz
};


// processed state ready to send to sim 
// using 12 dimensional usual drone state shi might change later 
struct state = {
    float x, 
    float y, 
    float z, 
    float vx, 
    float vy, 
    float vz, 
    float gx, 
    float gy, 
    float gz, 
    float p, 
    float q, 
    float r
};