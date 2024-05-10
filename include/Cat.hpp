#ifndef CAT_HPP
#define CAT_HPP

#include "EnemyAttr.hpp"
#include "Entity.hpp"
#include "RandomGenerator.hpp"
#include "SharedAnimation.hpp"
#include <array>

enum class CatType : size_t {
    CAT_TOWER = 0,
    CAT,
    TANK_CAT,
    AXE_CAT,
    CRAZED_GROSS_CAT,
    COW_CAT,
    BIRD_CAT,
    FISH_CAT,
    LIZARD_CAT,
    TITAN_CAT,
    CAT_TYPE_COUNT
};
static_assert(std::is_same_v<std::underlying_type_t<CatType>, size_t>);

class Cat : public Entity {
public:
    friend class CatFactory;

    struct Animation {
        std::unique_ptr<SharedRc::SharedAnimatedGameObject> walk;
        std::unique_ptr<SharedRc::SharedAnimatedGameObject> attack;
        std::unique_ptr<SharedRc::SharedAnimatedGameObject> idle;
        std::unique_ptr<SharedRc::SharedAnimatedGameObject> knockback;
    };

    Cat(CatType type, int level);

    void StartAttack();

    void Draw(Util::Transform trans);

    void UpdateTimer(double dt);

    void Walk(double dt);

    void DealDamage(Entity &e) override;

    [[nodiscard]]
    CatType GetCatType() const;

    bool OnAttack();

    void ResetState();

    void Move(float dx);

    void SetY(float low, float high);

private:
    inline static RandomFloatGenerator s_Random;

    void Attack();
    void CoolDownComplete();
    void OnHitBack() override;
    void LoadResource();

    [[nodiscard]]
    HitBox ToWorldSpace(HitBox hitbox) const override;

    CatType m_Type;
    bool m_OnAttack = false;
    EntityState m_PrevDrawState = EntityState::HITBACK;

    float m_TargetY = 0.0f;

    double hb_vel_y = 0.0; // hitback velocity - y axis
    double hb_dy = 0.0;
    int land = 0;

    Animation m_Anime;
};

class CatAnimeResource {
    struct UtilAnime {
        std::unique_ptr<SharedRc::Animation> walk;
        std::unique_ptr<SharedRc::Animation> attack;
        std::unique_ptr<SharedRc::Animation> idle;
        std::unique_ptr<SharedRc::Animation> knockback;
    };

public:
    static void Init();

    static const Cat::Animation Get(CatType type);

private:
    static inline bool s_init = false;

    static inline std::vector<UtilAnime> s_anime;
};

//--------------------------------------------------------------------------
namespace BaseCatStats {

    inline const EntityStats CatTower = [] {
        EntityStats stats;
        stats.health = 250;
        stats.kb = 1;
        stats.single_target = true;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "CatTower";
#endif
        return stats;
    }();

    inline const EntityStats Cat = [] {
        EntityStats stats;
        stats.health = 10;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 100;
        stats.single_target = true;
        stats.atk_prep_time = 0.54;
        stats.atk_cool_down = 0.96;
        stats.recharge_time = 2000;
        stats.cost = 100;
        stats.det_box = {0, 46};
        stats.hit_box = {0, 46};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "Cat";
#endif
        return stats;
    }();

    inline const EntityStats TankCat = [] {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 30;
        stats.single_target = false;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 0.96;
        stats.recharge_time = 2000;
        stats.cost = 200;
        stats.det_box = {0, 10};
        stats.hit_box = {0, 10};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "TankCat";
#endif
        return stats;
    }();

    inline const EntityStats AxeCat = [] {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 100;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 100;
        stats.single_target = true;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 0.96;
        stats.recharge_time = 2000;
        stats.cost = 300;
        stats.det_box = {0, 140};
        stats.hit_box = {0, 140};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
        stats.strong = {EnemyAttr::RED};
#ifdef ENABLE_BATTLE_LOG
        stats.name = "AxeCat";
#endif
        return stats;
    }();

    inline EntityStats CrazedGrossCat = []() {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 100;
        stats.single_target = true;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 0.96;
        stats.recharge_time = 2000;
        stats.cost = 10;
        stats.det_box = {-140, 140};
        stats.hit_box = {-140, 140};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "CrazedGrossCat";
#endif
        return stats;
    }();

    inline EntityStats CowCat = []() {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 100;
        stats.single_target = true;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 0.96;
        stats.recharge_time = 2000;
        stats.cost = 10;
        stats.det_box = {-140, 140};
        stats.hit_box = {-140, 140};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "CowCat";
#endif
        return stats;
    }();

    inline EntityStats BirdCat = []() {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 100;
        stats.single_target = true;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 0.96;
        stats.recharge_time = 2000;
        stats.cost = 10;
        stats.det_box = {-140, 140};
        stats.hit_box = {-140, 140};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "BirdCat";
#endif
        return stats;
    }();

    inline EntityStats FishCat = []() {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 100;
        stats.single_target = true;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 0.96;
        stats.recharge_time = 2000;
        stats.cost = 10;
        stats.det_box = {-140, 140};
        stats.hit_box = {-140, 140};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "FishCat";
#endif
        return stats;
    }();

    inline EntityStats LizardCat = []() {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 100;
        stats.single_target = true;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 0.96;
        stats.recharge_time = 2000;
        stats.cost = 10;
        stats.det_box = {-140, 140};
        stats.hit_box = {-140, 140};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "LizardCat";
#endif
        return stats;
    }();

    inline EntityStats TitanCat = []() {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 100;
        stats.single_target = true;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 0.96;
        stats.recharge_time = 2000;
        stats.cost = 10;
        stats.det_box = {-140, 140};
        stats.hit_box = {-140, 140};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "TitanCat";
#endif
        return stats;
    }();

    inline const std::array<EntityStats,
                            static_cast<size_t>(CatType::CAT_TYPE_COUNT)>
        Stats = {BaseCatStats::CatTower,       BaseCatStats::Cat,
                 BaseCatStats::TankCat,        BaseCatStats::AxeCat,
                 BaseCatStats::CrazedGrossCat, BaseCatStats::CowCat,
                 BaseCatStats::BirdCat,        BaseCatStats::FishCat,
                 BaseCatStats::LizardCat,      BaseCatStats::TitanCat};

} // BaseCatStats
//-----------------------------------------------------------------------------
namespace CatAnime {

// clang-format off

    inline Cat::Animation Tower() {
        return CatAnimeResource::Get(CatType::CAT_TOWER);
    }

    inline Cat::Animation Cat() {
        auto cat = CatAnimeResource::Get(CatType::CAT);

        cat.walk->SetInterval(300); // ms
        cat.walk->SetLooping(true);

        cat.attack->SetInterval(BaseCatStats::Cat.atk_prep_time * 1000.0 / 3.0);
        cat.attack->SetLooping(false);
        
        return cat;
    }
    
    inline Cat::Animation Tank() {
        auto tank = CatAnimeResource::Get(CatType::TANK_CAT);

        tank.walk->SetInterval(300); // ms
        tank.walk->SetLooping(true);

        tank.attack->SetInterval(BaseCatStats::Cat.atk_prep_time * 1000.0 / 3.0);
        tank.attack->SetLooping(false);
        
        return tank;
    }
    
    inline Cat::Animation Axe() {
        auto axe = CatAnimeResource::Get(CatType::AXE_CAT);

        axe.walk->SetInterval(300); // ms
        axe.walk->SetLooping(true);

        axe.attack->SetInterval(BaseCatStats::Cat.atk_prep_time * 1000.0 / 4.0);
        axe.attack->SetLooping(false);

        return axe;
    }
    
    inline Cat::Animation Gross() {
        auto gross = CatAnimeResource::Get(CatType::CRAZED_GROSS_CAT);

        gross.walk->SetInterval(200); // ms
        gross.walk->SetLooping(true);

        gross.attack->SetInterval(BaseCatStats::Cat.atk_prep_time * 1000.0 / 3.0);
        gross.attack->SetLooping(false);
        
        return gross;
    }
    /*
    inline Cat::Animation Cow() {
        auto walk = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/004/Animation/walk0.png",
            RESOURCE_DIR "/cats/004/Animation/walk1.png",
            RESOURCE_DIR "/cats/004/Animation/walk2.png",
            RESOURCE_DIR "/cats/004/Animation/walk3.png"
        });
        walk->SetInterval(200); // ms
        walk->SetLooping(true);

        auto attack = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/004/Animation/attack0.png",
            RESOURCE_DIR "/cats/004/Animation/attack1.png",
            RESOURCE_DIR "/cats/004/Animation/attack1.png" // for padding
        });
        attack->SetInterval(BaseCatStats::Cat.atk_prep_time * 1000.0 / 2.0);
        attack->SetLooping(false);
        
        auto idle = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/004/Animation/idle.png"
        });

        auto knockback = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{RESOURCE_DIR "/cats/004/Animation/knockback.png"});

        Cat::Animation a;
        a.walk = std::move(walk);
        a.attack = std::move(attack);
        a.idle = std::move(idle);
        a.knockback = std::move(knockback);
        return a;
    }

    inline Cat::Animation Bird() {
        auto walk = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/005/Animation/walk0.png",
            RESOURCE_DIR "/cats/005/Animation/walk1.png",
            RESOURCE_DIR "/cats/005/Animation/walk2.png"
        });
        walk->SetInterval(200); // ms
        walk->SetLooping(true);

        auto attack = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/005/Animation/attack0.png",
            RESOURCE_DIR "/cats/005/Animation/attack1.png",
            RESOURCE_DIR "/cats/005/Animation/attack2.png",
            RESOURCE_DIR "/cats/005/Animation/attack3.png",
            RESOURCE_DIR "/cats/005/Animation/attack3.png" // for padding
        });
        attack->SetInterval(BaseCatStats::Cat.atk_prep_time * 1000.0 / 4.0);
        attack->SetLooping(false);
        
        auto idle = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/005/Animation/walk0.png",
            RESOURCE_DIR "/cats/005/Animation/walk1.png",
            RESOURCE_DIR "/cats/005/Animation/walk2.png"
        });
        walk->SetInterval(200); // ms
        walk->SetLooping(true);

        auto knockback = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{RESOURCE_DIR "/cats/005/Animation/knockback.png"});

        Cat::Animation a;
        a.walk = std::move(walk);
        a.attack = std::move(attack);
        a.idle = std::move(idle);
        a.knockback = std::move(knockback);
        return a;
    }

    inline Cat::Animation Fish() {
        auto walk = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/006/Animation/idle.png",
            RESOURCE_DIR "/cats/006/Animation/walk0.png",
            RESOURCE_DIR "/cats/006/Animation/walk1.png",
            RESOURCE_DIR "/cats/006/Animation/walk2.png",
            RESOURCE_DIR "/cats/006/Animation/walk3.png",
            RESOURCE_DIR "/cats/006/Animation/walk4.png",
            RESOURCE_DIR "/cats/006/Animation/walk5.png",
            RESOURCE_DIR "/cats/006/Animation/walk6.png"
        });
        walk->SetInterval(200); // ms
        walk->SetLooping(true);

        auto attack = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/006/Animation/attack0.png",
            RESOURCE_DIR "/cats/006/Animation/attack1.png",
            RESOURCE_DIR "/cats/006/Animation/attack2.png",
            RESOURCE_DIR "/cats/006/Animation/attack2.png" // for padding
        });
        attack->SetInterval(BaseCatStats::Cat.atk_prep_time * 1000.0 / 3.0);
        attack->SetLooping(false);
        
        auto idle = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/006/Animation/idle.png",
        });

        auto knockback = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{RESOURCE_DIR "/cats/006/Animation/attack0.png"});

        Cat::Animation a;
        a.walk = std::move(walk);
        a.attack = std::move(attack);
        a.idle = std::move(idle);
        a.knockback = std::move(knockback);
        return a;
    }

    inline Cat::Animation Lizard() {
        auto walk = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/007/Animation/idle.png",
            RESOURCE_DIR "/cats/007/Animation/walk0.png",
            RESOURCE_DIR "/cats/007/Animation/walk1.png",
            RESOURCE_DIR "/cats/007/Animation/walk2.png",
            RESOURCE_DIR "/cats/007/Animation/walk3.png"
        });
        walk->SetInterval(200); // ms
        walk->SetLooping(true);

        auto attack = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/007/Animation/attack0.png",
            RESOURCE_DIR "/cats/007/Animation/attack1.png",
            RESOURCE_DIR "/cats/007/Animation/attack2.png",
            RESOURCE_DIR "/cats/007/Animation/attack3.png",
            RESOURCE_DIR "/cats/007/Animation/attack4.png",
            RESOURCE_DIR "/cats/007/Animation/attack4.png" // for padding
        });
        attack->SetInterval(BaseCatStats::Cat.atk_prep_time * 1000.0 / 5.0);
        attack->SetLooping(false);
        
        auto idle = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/007/Animation/idle.png",
        });

        auto knockback = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{RESOURCE_DIR "/cats/007/Animation/attack0.png"});

        Cat::Animation a;
        a.walk = std::move(walk);
        a.attack = std::move(attack);
        a.idle = std::move(idle);
        a.knockback = std::move(knockback);
        return a;
    }

    inline Cat::Animation Titan() {
        auto walk = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/008/Animation/walk0.png",
            RESOURCE_DIR "/cats/008/Animation/walk1.png",
            RESOURCE_DIR "/cats/008/Animation/walk2.png",
            RESOURCE_DIR "/cats/008/Animation/walk3.png",
            RESOURCE_DIR "/cats/008/Animation/walk4.png",
            RESOURCE_DIR "/cats/008/Animation/walk5.png",
            RESOURCE_DIR "/cats/008/Animation/walk6.png",
        });
        walk->SetInterval(200); // ms
        walk->SetLooping(true);

        auto attack = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/008/Animation/attack0.png",
            RESOURCE_DIR "/cats/008/Animation/attack1.png",
            RESOURCE_DIR "/cats/008/Animation/attack2.png",
            RESOURCE_DIR "/cats/008/Animation/attack3.png",
            RESOURCE_DIR "/cats/008/Animation/attack4.png",
            RESOURCE_DIR "/cats/008/Animation/attack5.png",
            RESOURCE_DIR "/cats/008/Animation/attack6.png",
            RESOURCE_DIR "/cats/008/Animation/attack6.png" // for padding
        });
        attack->SetInterval(BaseCatStats::Cat.atk_prep_time * 1000.0 / 5.0);
        attack->SetLooping(false);
        
        auto idle = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/008/Animation/idle.png",
        });

        auto knockback = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{RESOURCE_DIR "/cats/008/Animation/hitback.png"});

        Cat::Animation a;
        a.walk = std::move(walk);
        a.attack = std::move(attack);
        a.idle = std::move(idle);
        a.knockback = std::move(knockback);
        return a;
    }
    */
// clang-format on

} // namespace CatAnime

#endif // CAT_HPP