#!/bin/bash
TELEGRAM_BOT_TOKEN="5853670582:AAFJk2Rdqb_D17nWpmdQ0wfQEnRK8dVoJbo"
URL="https://api.telegram.org/bot$TELEGRAM_BOT_TOKEN/sendMessage"
chat_id="chat_id=1702899851"


curl -q "$URL?$chat_id&text=Nickname:+$7%0AProject:+$4%0ABranch:+$5%0ACommit+messege:+$6+(pipeline+ID:+$3)%0AStage:+$1%0A%0AStatus:+$2"
