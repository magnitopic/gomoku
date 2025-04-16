from constants import *


def config_input():
    try:
        value = input(f"{T_RESET}>> ")
        if value == "":
            print("Default value")
    except EOFError:
        print()
        exit()
    return value.strip().lower()


def initial_game_config() -> dict[str, str | bool]:
    config = {
        "ai": True,
        "difficulty": "standard",
        "board_size": "19",
        "save_history": False,
        "game_mode": "subject"
    }

    print(f"\n{T_CYAN}-- Welcome to GOMOKU ⚫️⚪️ --")
    print()
    print(f"{T_YELLOW}Please select your game configuration:")

    # VS AI or VS Player
    print(
        f"\n{T_BLUE}1. Would you like to play against the AI? ({T_GREEN}Y{T_BLUE}/n)")
    ai_choice = config_input()

    if ai_choice == "n":
        config["ai"] = False
        config["difficulty"] = None
    elif ai_choice != "y" and ai_choice != "":
        print(f"{T_RED}Invalid choice. Defaulting to AI mode.")
        config["ai"] = True

    # AI Difficulty
    if config["ai"]:
        print(
            f"\n{T_BLUE}2. Choose AI difficulty: (easy/middle/{T_GREEN}standard{T_BLUE})")
        ai_difficulty = config_input()

        if ai_difficulty in ["easy", "middle", "standard"]:
            config["difficulty"] = ai_difficulty
        elif ai_difficulty != "":
            print(f"{T_RED}Invalid choice. Defaulting to standard difficulty.")
            config["difficulty"] = "standard"

    # Board Size
    print(f"\n{T_BLUE}3. Choose board size: (13/15/{T_GREEN}19{T_BLUE})")
    board_size = config_input()

    if board_size in ["13", "15", "19"]:
        config["board_size"] = board_size
    elif board_size != "":
        print(f"{T_RED}Invalid choice. Defaulting to 19x19.")
        config["board_size"] = "19"

    # Save Game History
    print(
        f"\n{T_BLUE}4. Would you like to save game history? (y/{T_GREEN}N{T_BLUE})")
    save_history = config_input()

    if save_history == "y":
        config["save_history"] = True
    elif save_history != "n" and save_history != "":
        print(f"{T_RED}Invalid choice. Defaulting to not saving history.")
        config["save_history"] = False

    # Game Mode
    print(
        f"\n{T_BLUE}5. Choose game mode: (standard/{T_GREEN}subject{T_BLUE})")
    game_mode = config_input()
    if game_mode in ["subject", "standard"]:
        config["game_mode"] = game_mode
    elif game_mode != "":
        print(f"{T_RED}Invalid choice. Defaulting to standard mode.")
        config["game_mode"] = "standard"
    print(f"\n{T_GREEN}Game configuration complete!{T_RESET}")
    print(f"{T_BLUE}Starting the game...\n")

    print(T_WHITE, "-"*20, T_GRAY)

    return config


if __name__ == "__main__":
    config = initial_game_config()
    print("Config:", config)
