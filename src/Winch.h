#ifndef WINCH_H
#define WINCH_H

class Winch {
    public:
        Winch();
        void begin();
        void command();
    private:
        unsigned long prevTime;
        float prevError;
        int pos; 
        int posCommand;
        float cumError;
};

#endif /* WINCH_H */
