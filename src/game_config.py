import tkinter as tk
from tkinter import ttk


def game_config_popup():
    config_window = tk.Tk()
    config_window.title("Gomoku Configuration")
    config_window.geometry("300x450")
    config_window.resizable(False, False)

    padding = {"padx": 10, "pady": 5}

    main_frame = ttk.Frame(config_window)
    main_frame.pack(fill="both", expand=True, padx=20, pady=20)

    # AI Difficulty
    difficulty_frame = ttk.LabelFrame(main_frame, text="AI Difficulty")
    difficulty_frame.pack(fill="x", **padding)

    difficulty_var = tk.StringVar(value="standard")
    ttk.Radiobutton(difficulty_frame, text="Easy",
                    variable=difficulty_var, value="easy").pack(anchor="w")
    ttk.Radiobutton(difficulty_frame, text="Middle",
                    variable=difficulty_var, value="middle").pack(anchor="w")
    ttk.Radiobutton(difficulty_frame, text="Standard",
                    variable=difficulty_var, value="standard").pack(anchor="w")

    # Board Size
    board_frame = ttk.LabelFrame(main_frame, text="Board Size")
    board_frame.pack(fill="x", **padding)

    board_size_var = tk.StringVar(value="19x19")
    ttk.Radiobutton(board_frame, text="13x13",
                    variable=board_size_var, value="13x13").pack(anchor="w")
    ttk.Radiobutton(board_frame, text="15x15",
                    variable=board_size_var, value="15x15").pack(anchor="w")
    ttk.Radiobutton(board_frame, text="19x19",
                    variable=board_size_var, value="19x19").pack(anchor="w")

    # Save Game History
    save_history_frame = ttk.LabelFrame(main_frame, text="Game History")
    save_history_frame.pack(fill="x", **padding)

    save_history_var = tk.BooleanVar(value=False)
    ttk.Checkbutton(save_history_frame, text="Save Game History",
                    variable=save_history_var).pack(anchor="w")

    # Game Mode
    game_mode_frame = ttk.LabelFrame(main_frame, text="Game Mode")
    game_mode_frame.pack(fill="x", **padding)

    game_mode_var = tk.StringVar(value="subject")
    ttk.Radiobutton(game_mode_frame, text="Subject",
                    variable=game_mode_var, value="subject").pack(anchor="w")
    ttk.Radiobutton(game_mode_frame, text="Standard",
                    variable=game_mode_var, value="standard").pack(anchor="w")

    config_result = {}

    def on_ok():
        config_result["difficulty"] = difficulty_var.get()
        config_result["board_size"] = board_size_var.get()
        config_result["save_history"] = save_history_var.get()
        config_result["game_mode"] = game_mode_var.get()
        """ config_window.destroy() """
        config_window.quit()

    def on_cancel():
        config_window.quit()

    button_frame = ttk.Frame(main_frame)
    button_frame.pack(fill="x", pady=10)

    ttk.Button(button_frame, text="Play Gomoku",
               command=on_ok).pack(side="right", padx=5)
    ttk.Button(button_frame, text="Exit", command=on_cancel).pack(side="right")

    # Center the window on the screen
    config_window.update_idletasks()
    width = config_window.winfo_width()
    height = config_window.winfo_height()
    x = (config_window.winfo_screenwidth() // 2) - (width // 2)
    y = (config_window.winfo_screenheight() // 2) - (height // 2)
    config_window.geometry('{}x{}+{}+{}'.format(width, height, x, y))

    config_window.bind("<Escape>", lambda event: on_cancel())

    config_window.mainloop()

    return config_result


if __name__ == "__main__":
    config = game_config_popup()
    print("Config:", config)
