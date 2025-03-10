#!/bin/bash

echo "<!DOCTYPE html>"
echo "<html lang=\"de\">"
echo "  <head>"
echo "    <meta charset=\"UTF-8\">"
echo "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
echo "    <title>System Status</title>"
echo "    <style>"
echo "      body {"
echo "        font-family: Arial, sans-serif;"
echo "        background-color: #f4f4f4;"
echo "        display: flex;"
echo "        justify-content: center;"
echo "        align-items: center;"
echo "        height: 100vh;"
echo "        margin: 0;"
echo "        color: #333;"
echo "      }"
echo "      .container {"
echo "        background-color: #fff;"
echo "        width: 600px;"
echo "        border-radius: 8px;"
echo "        box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);"
echo "        padding: 30px;"
echo "        text-align: center;"
echo "      }"
echo "      h1 {"
echo "        font-size: 2.5em;"
echo "        color: #4CAF50;"
echo "        margin-bottom: 20px;"
echo "      }"
echo "      h2 {"
echo "        font-size: 1.5em;"
echo "        margin: 20px 0 10px;"
echo "        color: #333;"
echo "      }"
echo "      pre {"
echo "        background-color: #f2f2f2;"
echo "        padding: 15px;"
echo "        border-radius: 5px;"
echo "        text-align: left;"
echo "        font-size: 1.1em;"
echo "        line-height: 1.5;"
echo "        color: #333;"
echo "        overflow-x: auto;"
echo "      }"
echo "      footer {"
echo "        margin-top: 30px;"
echo "        font-size: 0.9em;"
echo "        color: #777;"
echo "      }"
echo "    </style>"
echo "  </head>"
echo "  <body>"
echo "    <div class=\"container\">"
echo "      <h1>System Status</h1>"
echo "      <h2>Uptime</h2>"
echo "      <pre>$(uptime -p)</pre>"
echo "      <h2>CPU Usage</h2>"
            CPU_IDLE=$(top -bn1 | grep "Cpu(s)" | sed "s/.*, *\([0-9.]*\)%* id.*/\1/" | awk '{print $1}')
            CPU_USAGE=$(echo "100 - $CPU_IDLE" | bc)
echo "      <pre>CPU Usage: $CPU_USAGE%</pre>"
echo "      <h2>Memory Usage</h2>"
echo "      <pre>$(free -h)</pre>"
echo "      <h2>Disk Usage</h2>"
echo "      <pre>$(df -h | grep -E '^/dev/')</pre>"
echo "      <footer>Systemstatus abgefragt am $(date)</footer>"
echo "    </div>"
echo "  </body>"
echo "</html>"
