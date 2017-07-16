#pragma once

#include <deque>

class GameSound {
protected:
    GameSound() {
    }
    ~GameSound() {
    }
public:
    class sound {
    public:
        sound( ) {
        }
        ~sound() {
        }
        int open( float * volume, const char* path, int nMax = 2, int loop = 0 ) {
            return 1;
        }
        bool isOpen() {
            return true;
        }
        int play( int lr = 0) {
            return 1;
        }
        int stop() {
            return 1;
        }
        void setVolume( float volume ) {
        }
    private:
        int mIndex;
        int mMax;
        bool mOpen;
        float * mVolume;
    };
    static GameSound& ins() {
        static GameSound gamesound;
        return gamesound;
    }
    void loadSFX () {
#if !defined(XP_RELEASE)
        return;
#endif
        std::string basePath = "sound/sfx/default/";
        mSFX_ko.open(&mVolume, (basePath + "sfx_ko.wav").c_str());
        mSFX_gameover.open(&mVolume, (basePath + "sfx_gameover.wav").c_str());
        mSFX_pc.open(&mVolume, (basePath + "sfx_perfectclear.wav").c_str());
        mSFX_move.open(&mVolume, (basePath + "sfx_move.wav").c_str(), 4);
        mSFX_hold.open(&mVolume, (basePath + "sfx_hold.wav").c_str());
        mSFX_rotate.open(&mVolume, (basePath + "sfx_rotate.wav").c_str());
        mSFX_softdrop.open(&mVolume, (basePath + "sfx_softdrop.wav").c_str(), 8);
        mSFX_harddrop.open(&mVolume, (basePath + "sfx_harddrop.wav").c_str());
        mSFX_lockdown.open(&mVolume, (basePath + "sfx_lockdown.wav").c_str());
        mSFX_lineattack.open(&mVolume, (basePath + "sfx_lineattack.wav").c_str());
        mSFX_clears[0].open(&mVolume, (basePath + "sfx_single.wav").c_str());
        mSFX_clears[1].open(&mVolume, (basePath + "sfx_double.wav").c_str());
        mSFX_clears[2].open(&mVolume, (basePath + "sfx_triple.wav").c_str());
        mSFX_clears[3].open(&mVolume, (basePath + "sfx_tetris.wav").c_str());
        mSFX_b2b_tetris.open(&mVolume, (basePath + "sfx_b2b_tetris.wav").c_str());
        for ( int i = 0; i < 20; ++i) {
            char name[16];
            sprintf( name, "sfx_combo%d.wav", i + 1);
            mSFX_combo[i].open(&mVolume, (basePath + name).c_str());
        }
        mSFX_tspin[0].open(&mVolume, (basePath + "sfx_tspin_mini.wav").c_str());
        mSFX_tspin[1].open(&mVolume, (basePath + "sfx_tspin_single.wav").c_str());
        mSFX_tspin[2].open(&mVolume, (basePath + "sfx_tspin_double.wav").c_str());
        mSFX_tspin[3].open(&mVolume, (basePath + "sfx_tspin_triple.wav").c_str());
        mSFX_b2b_tspin[0].open(&mVolume, (basePath + "sfx_b2b_tspin_mini.wav").c_str());
        mSFX_b2b_tspin[1].open(&mVolume, (basePath + "sfx_b2b_tspin_single.wav").c_str());
        mSFX_b2b_tspin[2].open(&mVolume, (basePath + "sfx_b2b_tspin_double.wav").c_str());
        mSFX_b2b_tspin[3].open(&mVolume, (basePath + "sfx_b2b_tspin_triple.wav").c_str());
    }
    void loadBGM_wait( AI::Random& rnd ) {
    }
    void loadBGM( AI::Random& rnd ) {
    }
    void stopBGM() {
    }
    void setVolume( float volume ) {
    }
    void setVolumeAdd( float add ) {
    }
    float getVolume() const {
    }
public:
    sound mSFX_move;
    sound mSFX_hold;
    sound mSFX_rotate;
    sound mSFX_softdrop;
    sound mSFX_harddrop;
    sound mSFX_lockdown;
    sound mSFX_lineattack;
    sound mSFX_clears[4];
    sound mSFX_combo[20];
    sound mSFX_tspin[4];
    sound mSFX_b2b_tspin[4];
    sound mSFX_b2b_tetris;
    sound mSFX_ko;
    sound mSFX_gameover;
    sound mSFX_pc;
    sound mbgm;
    float mVolume;
    float mBgmVolume;
};
