#pragma once

#ifndef PR_GAMETAG_CONSTS_H
#define PR_GAMETAG_CONSTS_H

// consts
#define SERVER_IP "127.0.0.1"
#define PORT 9990

#define CLIENTS_COUNT 2

#define FPS 30
#define FRAME_TIME (1000/FPS)

#define LOCK pthread_mutex_lock(&semaphore);
#define UNLOCK pthread_mutex_unlock(&semaphore);

#define MAP_SIZE_X 600
#define MAP_SIZE_Y 400

#define PLAYER_SIZE 32

#endif