#pragma once
#include <bitset>

enum class CollisionLayer {
    None,
    Player,
    PlayerBullet,
    Enemy,
    EnemyBullet,
    LastLayer,
};

constexpr auto LayersNumber = static_cast<int>(CollisionLayer::LastLayer);

class LayersCollisionSettings {
    static std::bitset<LayersNumber> layers[LayersNumber];
    static bool initialized;

    static void SetLayersCollision(CollisionLayer firstLayer, CollisionLayer secondLayer) {
        layers[static_cast<int>(firstLayer)].set(static_cast<int>(secondLayer));
        layers[static_cast<int>(secondLayer)].set(static_cast<int>(firstLayer));
    }

    static void Init() {
        SetLayersCollision(CollisionLayer::Player, CollisionLayer::EnemyBullet);
        SetLayersCollision(CollisionLayer::Enemy, CollisionLayer::PlayerBullet);
        SetLayersCollision(CollisionLayer::Player, CollisionLayer::Enemy);
    }

public:
    LayersCollisionSettings() = delete;
    ~LayersCollisionSettings() = delete;

    static bool IsLayersCollides(CollisionLayer firstLayer, CollisionLayer secondLayer) {
        if (!initialized) {
            Init();
            initialized = true;
        }
        return layers[static_cast<int>(firstLayer)].test(static_cast<int>(secondLayer));
    }
};

bool LayersCollisionSettings::initialized;
std::bitset<LayersNumber> LayersCollisionSettings::layers[LayersNumber];
