#pragma once

#include "Enemy.hpp"
#include "Cat.hpp"
#include "Util/Image.hpp"
#include "AnimatedGameObject.hpp"

template<typename EntityType>
class Entity {
public:
    /*static Entity<CatType>
    CreateCat(const EntityStats &stats, CatType type, const CatStatsMat &mat,
              int level,
              std::function<void(const Entity<CatType> &)> hit_callback);

    static Entity<EnemyType>
    CreateEnemy(const EntityStats &stats, EnemyType type, float stats_modifier,
                std::function<void(const Entity<EnemyType> &)> hit_callback);*/

    void GetHit(int damage, std::optional<EnemyAttr> attr);

    int GetDamage(std::optional<EnemyAttr> attr) const;

    std::optional<EnemyAttr> GetEnemyAttr() const;

    EntityType GetEntityType() const;

    HitBox GetHitBox() const;

    void Draw(Util::Image &image) const;
//--------------------------------------------------------------------------
    bool IsEnemyInRange();

     //Walk -> Idle -> Attack -> calculate damage -> Hitback
    // -> Update animation

    void Hit(int damage,Entity enemy){};

    void Walk(){
        if(m_State != EntityState::WALK){
            return;
        }
        if(IsEnemyInRange()){
            m_State = EntityState::IDLE;
            m_walk->Reset();
            return;
        }
        //TODO:MovePosition(speed/2,y)
        if(IsEnemyInRange()){
            m_State = EntityState::IDLE;
            m_walk->Reset();
            return;
        }
    };

    void Idle(){
        if(m_State != EntityState::IDLE){
            return;
        }
        if(!IsEnemyInRange()){
            m_State = EntityState::WALK;
            m_idle->Reset();
            return;
        }
        if(AttackCD() == 0){
            m_State = EntityState::ATTACK;
            m_idle->Reset();
            return;
        }
    }

    void Attack(){
        if(m_State != EntityState::ATTACK) {
            return;
        }
        //The unit should use frame(1/30s) instead of second.
        if(m_Stats.atk_prep_time == m_attack->GetCurrentFrameIndex()){
            Hit();
        }
    }

    void CalculateDamage(){

    };

    void HitBack(){
        if(m_State != EntityState::HITBACK) {
            return;
        }
    }

    void Update(){
        switch (m_State) {
            case EntityState::ATTACK:{
                m_attack->Play();
                break;
            }
            case EntityState::IDLE:{
                m_idle->Play();
                break;
            }
            case EntityState::WALK:{
                m_walk->Play();
                break;
            }
            case EntityState::HITBACK:{
                m_hitback->Play();
                break;
            }
        }
    };

protected:
    EntityState m_State = EntityState::WALK;
    EntityStats m_Stats;
    bool is_enemy = false;
    std::shared_ptr<AnimatedGameObject> m_attack;
    std::shared_ptr<AnimatedGameObject> m_walk;
    std::shared_ptr<AnimatedGameObject> m_idle;
    std::shared_ptr<AnimatedGameObject> m_hitback;
};
