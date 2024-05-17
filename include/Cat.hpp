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
    ACTRESS_CAT,
    KUNG_FU_CAT,
    MR_CAT,
    BONDAGE_CAT,
    DOM_CAT,
    CATS_IN_A_BOX,
    SKIRT_CAT,
    MONEKO,
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

    void GetHit(int damage, const Entity &attacker) override;

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
        stats.damage = 1;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 100;
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
        stats.damage_diff = 0;
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

    inline EntityStats ActressCat = []() {
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
        stats.name = "ActressCat";
#endif
        return stats;
    }();

    inline EntityStats KungFuCat = []() {
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
        stats.name = "KungFuCat";
#endif
        return stats;
    }();

    inline EntityStats MrCat = []() {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 100;
        stats.single_target = true;
        stats.atk_prep_time = 0.07;
        stats.atk_cool_down = 0.53;
        stats.recharge_time = 2000;
        stats.cost = 10;
        stats.det_box = {-140, 140};
        stats.hit_box = {-140, 140};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "Mr.";
#endif
        return stats;
    }();

    inline EntityStats BondageCat = []() {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 100;
        stats.single_target = true;
        stats.atk_prep_time = 0.2;
        stats.atk_cool_down = 1.3;
        stats.recharge_time = 2000;
        stats.cost = 10;
        stats.det_box = {-140, 140};
        stats.hit_box = {-140, 140};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "BondageCat";
#endif
        return stats;
    }();

    inline EntityStats DomCat = []() {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 100;
        stats.single_target = true;
        stats.atk_prep_time = 0.2;
        stats.atk_cool_down = 0.57;
        stats.recharge_time = 2000;
        stats.cost = 10;
        stats.det_box = {-140, 140};
        stats.hit_box = {-140, 140};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "DomCat";
#endif
        return stats;
    }();

    inline EntityStats CatInBox = []() {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 100;
        stats.single_target = true;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 3.1;
        stats.recharge_time = 2000;
        stats.cost = 10;
        stats.det_box = {-140, 140};
        stats.hit_box = {-140, 140};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "CatInBox";
#endif
        return stats;
    }();

    inline EntityStats SkirtCat = []() {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 100;
        stats.single_target = true;
        stats.atk_prep_time = 0.2;
        stats.atk_cool_down = 0.2;
        stats.recharge_time = 2000;
        stats.cost = 10;
        stats.det_box = {-140, 140};
        stats.hit_box = {-140, 140};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "SkirtCat";
#endif
        return stats;
    }();

    inline EntityStats Moneko = []() {
        EntityStats stats;
        stats.health = 250;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 100;
        stats.single_target = true;
        stats.atk_prep_time = 0.93;
        stats.atk_cool_down = 3.3;
        stats.recharge_time = 2000;
        stats.cost = 10;
        stats.det_box = {-140, 140};
        stats.hit_box = {-140, 140};
        stats.attr = std::nullopt;
        stats.base_level = 1;
        stats.health_diff = 100;
        stats.damage_diff = 100;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "Moneko";
#endif
        return stats;
    }();

    inline const std::array<EntityStats,
                            static_cast<size_t>(CatType::CAT_TYPE_COUNT)>
        Stats = {BaseCatStats::CatTower,       BaseCatStats::Cat,
                 BaseCatStats::TankCat,        BaseCatStats::AxeCat,
                 BaseCatStats::CrazedGrossCat, BaseCatStats::CowCat,
                 BaseCatStats::BirdCat,        BaseCatStats::FishCat,
                 BaseCatStats::LizardCat,      BaseCatStats::TitanCat,
                 BaseCatStats::ActressCat,     BaseCatStats::KungFuCat,
                 BaseCatStats::MrCat,          BaseCatStats::BondageCat,
                 BaseCatStats::DomCat,         BaseCatStats::CatInBox,
                 BaseCatStats::SkirtCat,       BaseCatStats::Moneko};

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

        tank.attack->SetInterval(BaseCatStats::TankCat.atk_prep_time * 1000.0 / 3.0);
        tank.attack->SetLooping(false);
        
        return tank;
    }
    
    inline Cat::Animation Axe() {
        auto axe = CatAnimeResource::Get(CatType::AXE_CAT);

        axe.walk->SetInterval(300); // ms
        axe.walk->SetLooping(true);

        axe.attack->SetInterval(BaseCatStats::AxeCat.atk_prep_time * 1000.0 / 4.0);
        axe.attack->SetLooping(false);

        return axe;
    }
    
    inline Cat::Animation Gross() {
        auto gross = CatAnimeResource::Get(CatType::CRAZED_GROSS_CAT);

        gross.walk->SetInterval(200); // ms
        gross.walk->SetLooping(true);

        gross.attack->SetInterval(BaseCatStats::CrazedGrossCat.atk_prep_time * 1000.0 / 3.0);
        gross.attack->SetLooping(false);
        
        return gross;
    }
    
    inline Cat::Animation Cow() {
        auto cow = CatAnimeResource::Get(CatType::COW_CAT);

        cow.walk->SetInterval(200); // ms
        cow.walk->SetLooping(true);

        cow.attack->SetInterval(BaseCatStats::CowCat.atk_prep_time * 1000.0 / 2.0);
        cow.attack->SetLooping(false);

        return cow;
    }
    
    inline Cat::Animation Bird() {
        auto bird = CatAnimeResource::Get(CatType::BIRD_CAT);

        bird.walk->SetInterval(200); // ms
        bird.walk->SetLooping(true);

        bird.attack->SetInterval(BaseCatStats::BirdCat.atk_prep_time * 1000.0 / 4.0);
        bird.attack->SetLooping(false);
        
        bird.walk->SetInterval(200); // ms
        bird.walk->SetLooping(true);

        return bird;
    }
    
    inline Cat::Animation Fish() {
        auto fish = CatAnimeResource::Get(CatType::FISH_CAT);

        fish.walk->SetInterval(200); // ms
        fish.walk->SetLooping(true);

        fish.attack->SetInterval(BaseCatStats::FishCat.atk_prep_time * 1000.0 / 3.0);
        fish.attack->SetLooping(false);
        
        return fish;
    }
    
    inline Cat::Animation Lizard() {
        auto lizard = CatAnimeResource::Get(CatType::LIZARD_CAT);

        lizard.walk->SetInterval(200); // ms
        lizard.walk->SetLooping(true);

        lizard.attack->SetInterval(BaseCatStats::LizardCat.atk_prep_time * 1000.0 / 5.0);
        lizard.attack->SetLooping(false);
        
        return lizard;
    }
    
    inline Cat::Animation Titan() {
        auto titan = CatAnimeResource::Get(CatType::TITAN_CAT);
        
        titan.walk->SetInterval(200); // ms
        titan.walk->SetLooping(true);

        titan.attack->SetInterval(BaseCatStats::TitanCat.atk_prep_time * 1000.0 / 5.0);
        titan.attack->SetLooping(false);
        
        return titan;
    }

    inline Cat::Animation Actress() {
        auto actress = CatAnimeResource::Get(CatType::ACTRESS_CAT);
        
        actress.walk->SetInterval(200); // ms
        actress.walk->SetLooping(true);

        actress.attack->SetInterval(BaseCatStats::ActressCat.atk_prep_time * 1000.0 / 6.0);
        actress.attack->SetLooping(false);
        
        return actress;
    }
    
    inline Cat::Animation KungFu() {
        auto kung_fu = CatAnimeResource::Get(CatType::KUNG_FU_CAT);
        
        kung_fu.walk->SetInterval(200); // ms
        kung_fu.walk->SetLooping(true);

        kung_fu.attack->SetInterval(BaseCatStats::KungFuCat.atk_prep_time * 1000.0 / 4.0);
        kung_fu.attack->SetLooping(false);
        
        return kung_fu;
    }

    inline Cat::Animation Mr() {
        auto kung_fu = CatAnimeResource::Get(CatType::MR_CAT);
        
        kung_fu.walk->SetInterval(15); // ms
        kung_fu.walk->SetLooping(true);

        kung_fu.attack->SetInterval(BaseCatStats::MrCat.atk_cool_down * 1000.0 / 8.0);
        kung_fu.attack->SetLooping(false);
        
        return kung_fu;
    }

    inline Cat::Animation Bondage() {
        auto bondage = CatAnimeResource::Get(CatType::BONDAGE_CAT);
        
        bondage.walk->SetInterval(15); // ms
        bondage.walk->SetLooping(true);

        bondage.attack->SetInterval(BaseCatStats::BondageCat.atk_prep_time * 1000.0 / 5.0);
        bondage.attack->SetLooping(false);
        
        return bondage;
    }

    inline Cat::Animation Dom() {
        auto dom = CatAnimeResource::Get(CatType::DOM_CAT);
        
        dom.walk->SetInterval(100); // ms
        dom.walk->SetLooping(true);

        dom.attack->SetInterval(BaseCatStats::DomCat.atk_prep_time * 1000.0 / 3.0);
        dom.attack->SetLooping(false);
        
        return dom;
    }

    inline Cat::Animation CatInBox() {
        auto dom = CatAnimeResource::Get(CatType::CATS_IN_A_BOX);
        
        dom.walk->SetInterval(100); // ms
        dom.walk->SetLooping(true);

        dom.attack->SetInterval(BaseCatStats::CatInBox.atk_prep_time * 1000.0 / 5.0);
        dom.attack->SetLooping(false);
        
        return dom;
    }

    inline Cat::Animation Skirt() {
        auto dom = CatAnimeResource::Get(CatType::SKIRT_CAT);
        
        dom.walk->SetInterval(100); // ms
        dom.walk->SetLooping(true);

        dom.attack->SetInterval(BaseCatStats::SkirtCat.atk_prep_time * 1000.0 / 3.0);
        dom.attack->SetLooping(false);
        
        return dom;
    }

    inline Cat::Animation Moneko() {
        auto dom = CatAnimeResource::Get(CatType::MONEKO);
        
        dom.walk->SetInterval(100); // ms
        dom.walk->SetLooping(true);

        dom.attack->SetInterval(BaseCatStats::Moneko.atk_prep_time * 1000.0 / 11.0);
        dom.attack->SetLooping(false);
        
        return dom;
    }

// clang-format on

} // namespace CatAnime

#endif // CAT_HPP