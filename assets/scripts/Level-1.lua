---@diagnostic disable: undefined-global
Level = {
    assets = {
        { type = "texture", id = "tilemap-texture", file = "./assets/tilemaps/jungle.png" },
        { type = "texture", id = "chopper-texture", file = "./assets/images/chopper-spritesheet.png" },
        { type = "texture", id = "takeoff-texture", file = "./assets/images/takeoff-base.png" },
        { type = "texture", id = "tank-texture",    file = "./assets/images/tank-panther-right.png" },
        { type = "texture", id = "truck-texture",   file = "./assets/images/truck-ford-right.png" },
        { type = "texture", id = "radar-texture",   file = "./assets/images/radar.png" },
        { type = "texture", id = "bullet-texture",  file = "./assets/images/bullet.png" },
        { type = "music", id = "main-music-theme",  file = "./assets/music/Abnormal Circumstances.mp3" },
        { type = "soundEffect", id = "helicopter-sound",  file = "./assets/sounds/explosion4.wav" },
        { type = "font",    id = "charriot-font",   file = "./assets/fonts/charriot.ttf",            fontSize = 32 },
        { type = "font",    id = "pico-font-5",     file = "./assets/fonts/pico8.ttf",               fontSize = 5 },
        { type = "font",    id = "pico-font-10",    file = "./assets/fonts/pico8.ttf",               fontSize = 10 }
    },

    tilemap = {
        mapFile = "./assets/tilemaps/jungle.map",
        textureAssetId = "tilemap-texture",
        numRows = 20,
        numCols = 25,
        tileSize = 32,
        scale = 2.0
    },

    entities = {
        {
            --Player
            tag = "Player",
            components = {
                transform = {
                    position = { x = 240, y = 108 },
                    scale = { x = 2, y = 2 },
                    rotation = 0
                },
                velocity = {
                    velocity = { x = 0, y = 0 }
                },
                sprite = {
                    textureAssetId = "chopper-texture",
                    width = 32,
                    height = 32,
                    zIndex = 5
                },
                animation = {
                    frameRate = 8,
                    frames = {
                        {
                            { x = 0, y = 0, w = 32, h = 32 },
                            { x = 32, y = 0, w = 32, h = 32 },
                        },
                        {
                            { x = 0, y = 32, w = 32, h = 32 },
                            { x = 32, y = 32, w = 32, h = 32 },
                        },
                        {
                            { x = 0, y = 64, w = 32, h = 32 },
                            { x = 32, y = 64, w = 32, h = 32 },
                        },
                        {
                            { x = 0, y = 96, w = 32, h = 32 },
                            { x = 32, y = 96, w = 32, h = 32 },
                        }
                    }
                },
                boxCollider = {
                    offset = { x = 0, y = 0 },
                    width = 32,
                    height = 32,
                    collisionLayer = "Player"
                },
                health = {
                    health = 100,
                    maxHealth = 100
                },
                projectileEmitter = {
                    projectileVelocity = { x = 100, y = 100 },
                    collisionLayer = "PlayerBullet",
                    coolDown = 500,
                    lifeTime = 10000,
                    bulletDamage = 10,
                    autoShoot = false
                },
                direction = {
                    direction = { x = 0, y = -1 }
                },
                keyboardControl = {
                    upVector = { x = 0, y = -80 },
                    rightVector = { x = 80, y = 0 },
                    downVector = { x = 0, y = 80 },
                    leftVector = { x = -80, y = 0 },
                },
                cameraFollow = {},
                onUpdateScript = {
                    function(entity, deltaTime, elapsedTime)
                        print(string.format("Entity id: %d, delta time: %.8f, elapsed time: %d", entity:GetId(), deltaTime, elapsedTime))
                        SetPosition(entity, 500, 500)
                    end
                }
            }
        }
    }
}
