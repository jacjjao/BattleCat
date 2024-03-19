#ifndef ENTITY_HPP
#define ENTITY_HPP

#pragma once

#include "EntityStats.hpp"
#include "Util/Image.hpp"
#include "AnimatedGameObject.hpp"
#include "Timer.hpp"
#include <cassert>

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

    HitBox GetHitBox() const; // when attacking

    virtual void Draw(Util::Image &image) const = 0;

    EntityState GetState() const;

    bool IsInRange(const Entity &e) const;

//--------------------------------------------------------------------------
/*    bool IsEnemyInRange();

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
    };*/

    float GetPosX() const;

    bool IsSingleTarget() const;

    virtual void DealDamage(Entity &e) = 0;

    bool IsDead() const;

#ifdef ENABLE_BATTLE_LOG
    const std::string& GetName() const {
        return m_Stats.name;
    }
#endif // ENABLE_BATTLE_LOG

    static constexpr float s_KnockbackSpeed = 50.0f;
    static constexpr double s_KnockbackDuration = 1.0;

protected:
    void SetStats(const EntityStats &stats);
    void SetState(EntityState state);
    void OnUpdate();
    virtual HitBox ToWorldSpace(HitBox hitbox) const = 0;

    EntityStats m_Stats;
    float m_PosX = 0;
    bool m_IsEnemy = false;
    Timer m_AtkPrepTimer;
    Timer m_AtkCoolDownTimer;
    Timer m_KnockbackTimer;
    int m_KnockBackHealth = 0;
    int m_TotalDamage = 0;

    std::shared_ptr<AnimatedGameObject> m_attack;
    std::shared_ptr<AnimatedGameObject> m_walk;
    std::shared_ptr<AnimatedGameObject> m_idle;
    std::shared_ptr<AnimatedGameObject> m_hitback;

private:
    void ExitKnockbackState();

    EntityState m_State = EntityState::WALK;
};
#endif // ENTITY_HPP
