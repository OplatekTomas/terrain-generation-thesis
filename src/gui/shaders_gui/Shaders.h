//
// Created by tomas on 30.03.22.
//
#pragma once



class GUIShaders {
public:
    static const char *getLightningVS() {
        return lightning_vs;
    }

    static const char *getLightningFS() {
        return lightning_fs;
    }
    static const char *getSSAOFS() {
        return ssao_fs;
    }
    static const char* getSSAOVS(){
        return getLightningVS();
    }

    static const char* getSSAOBlurVS(){
        return getLightningVS();
    }

    static const char* getSSAOBlurFS(){
        return blur_fs;
    }

    static const char *lightning_vs;
    static const char *lightning_fs;
    static const char *ssao_fs;
    static const char *blur_fs;

};

