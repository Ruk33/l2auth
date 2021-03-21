#ifndef LIB_CONFIG_H
#define LIB_CONFIG_H

/**
 * Name max bytes.
 */
#define NAME_MAX_LENGTH 28

/**
 * Max characters a player can have.
 */
#define MAX_CHARACTERS 10

/**
 * Max character's name in bytes.
 */
#define CHARACTER_NAME_MAX_LEN 28

/**
 * Max character's title in bytes.
 */
#define CHARACTER_TITLE_MAX_LEN 28

/**
 * Range used to detect what
 * characters should receive a message
 * when a player talks.
 * If the range is too low, only characters
 * really close to the player talking will
 * receive the message.
 */
#define CONFIG_SHOUT_RANGE 800

#endif
