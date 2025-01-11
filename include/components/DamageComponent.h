#pragma once


struct DamageComponent {
    int damage;

    explicit DamageComponent(const int damage = 0)
        : damage(damage) {}
};