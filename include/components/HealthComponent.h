#pragma once


struct HealthComponent {
    int maxAmount;
    int amount;

    explicit HealthComponent(const int maxAmount = 0, const int amount = 0) : maxAmount{maxAmount}, amount{amount} {}
};
