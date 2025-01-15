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

inline CollisionLayer CollisionLayerFromString(const std::string& str) {
    if (str == "None") { return CollisionLayer::None; }
    if (str == "Player") { return CollisionLayer::Player; }
    if (str == "PlayerBullet") { return CollisionLayer::PlayerBullet; }
    if (str == "Enemy") { return CollisionLayer::Enemy; }
    if (str == "EnemyBullet") { return CollisionLayer::EnemyBullet; }
    return CollisionLayer::None;
}

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
