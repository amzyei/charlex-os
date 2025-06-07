# Charlex-Webshell

Welcome to Charlex-Webshell — a hacker-themed Python webshell designed for stealthy command and control.

## What’s inside?

- **Flask webshell** with a secret, random URL (UUID) to stay hidden.
- **Integrated C2 server** within the Flask app that listens on port 4444 and talks to clients using simple XOR encryption.
- **Client agent** that connects back, runs commands, and sends results.
- **Cool green-on-black terminal UI** for that hacker vibe.

## How to use

1. **Run the client agent on the target machine:**

```bash
python client_agent.py
```

*(Change the IP in client_agent.py if needed)*

2. **Launch the webshell (which includes the C2 server):**

```bash
python app.py
```

3. **Open your browser and go to:**

```
http://127.0.0.1:5000/<secret-uuid>
```

*(The UUID is shown in the console when you start the webshell)*

4. **Select the target client from the list and type commands, hit Enter, and watch the magic happen.**

## Important stuff

- This is for **authorized use only**. Don’t be a jerk.
- Communication uses simple XOR encryption — not bulletproof, but sneaky.
- The random URL helps keep things under the radar.
- Keep it safe, keep it smart.

## Files at a glance

- `app.py` — Flask webshell backend with integrated C2 server
- `client_agent.py` — The little agent that does your bidding
- `templates/shell.html` — The hacker-themed terminal UI
- `.gitignore` — Stuff to ignore in git

## License

Use it wisely.

## How to compile the client agent

If you want to compile the client agent into a standalone executable, you can do so manually using the following steps:

1. Create and activate a Python virtual environment:

```bash
python3 -m venv venv
source venv/bin/activate
```

2. Upgrade pip and install PyInstaller:

```bash
pip install --upgrade pip
pip install pyinstaller
```

3. Compile the client agent script:

```bash
pyinstaller --onefile client_agent.py
```

4. The compiled executable will be located in the `dist/` directory.

5. Deactivate the virtual environment when done:

```bash
deactivate
```

This replaces the previous `compile_client_agent.sh` script.
source venv/bin/activate
