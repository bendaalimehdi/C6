#include <Arduino.h>
#include "config_manager.h"
#include "leader_service.h"
#include "follower_service.h"
#include "logger.h"

// Déclarations globales
ConfigManager gConfigManager;
AppConfig gConfig;
LeaderService* gLeader = nullptr;
FollowerService* gFollower = nullptr;

void setup() {
    // Initialisation de la console série
    Serial.begin(115200);
    delay(1000); 

    Logger::info("=== FRIDGE MONITOR BOOT (ARDUINO MODE) ===");

    // Initialisation du système de fichiers LittleFS
    if (!gConfigManager.begin()) {
        Logger::error("LittleFS init failed");
        return;
    }

    // Chargement de la configuration depuis config.json
    if (!gConfigManager.load(gConfig)) {
        Logger::error("Config load failed: " + gConfigManager.getLastError());
        return;
    }

    Logger::info("Role identifie: " + gConfig.role);

    // Lancement du service selon le rôle
    if (gConfig.role == "leader") {
        gLeader = new LeaderService(gConfig);
        if (!gLeader->begin()) {
            Logger::error("Leader init failed");
        }
    } else if (gConfig.role == "follower") {
        gFollower = new FollowerService(gConfig);
        // Le follower effectue sa mesure, envoie les données et part en veille profonde
        gFollower->beginAndSleep();
    } else {
        Logger::error("Role invalide dans config.json");
    }
}

void loop() {
    // La boucle loop n'est réellement utilisée que par le mode Leader
    if (gLeader) {
        gLeader->loop();
    }
    
    // Petite pause pour laisser le processeur respirer (RTOS)
    delay(10); 
}