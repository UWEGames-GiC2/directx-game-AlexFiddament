#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "CMOGO.h"

class Projectiles : public CMOGO
{
public:
    Projectiles(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF);

    ~Projectiles();

    virtual void Tick(GameData* _GD) override;

protected:
};

#endif 

