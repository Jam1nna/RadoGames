
<style>
  @import url('https://fonts.googleapis.com/css2?family=VT323&family=Space+Mono:wght@400;700&display=swap');
  * { box-sizing: border-box; margin: 0; padding: 0; }
  .readme { font-family: 'Space Mono', monospace; color: var(--color-text-primary); padding: 2rem 0; max-width: 680px; }
  .header { text-align: center; margin-bottom: 2.5rem; }
  .glitch-title { font-family: 'VT323', monospace; font-size: 72px; line-height: 1; color: var(--color-text-primary); position: relative; display: inline-block; letter-spacing: 4px; }
  .glitch-title::before, .glitch-title::after { content: attr(data-text); position: absolute; top: 0; left: 0; width: 100%; height: 100%; }
  .glitch-title::before { color: #ff003c; animation: glitch1 3.5s infinite; clip-path: polygon(0 20%, 100% 20%, 100% 40%, 0 40%); }
  .glitch-title::after { color: #00e5ff; animation: glitch2 3.5s infinite; clip-path: polygon(0 55%, 100% 55%, 100% 75%, 0 75%); }
  @keyframes glitch1 { 0%,90%,100%{transform:none;opacity:0} 92%{transform:translateX(-4px) skewX(-2deg);opacity:.8} 94%{transform:translateX(3px);opacity:.8} 96%{transform:translateX(-2px);opacity:.8} 98%{transform:none;opacity:0} }
  @keyframes glitch2 { 0%,90%,100%{transform:none;opacity:0} 91%{transform:translateX(4px) skewX(3deg);opacity:.7} 93%{transform:translateX(-3px);opacity:.7} 95%{transform:translateX(2px);opacity:.7} 97%{transform:none;opacity:0} }
  .tagline { font-size: 13px; color: var(--color-text-secondary); letter-spacing: 6px; text-transform: uppercase; margin-top: 0.5rem; }
  .divider { display:flex; align-items:center; gap:10px; margin:2rem 0; opacity:.4; }
  .divider::before,.divider::after { content:''; flex:1; height:1px; background:var(--color-border-secondary); }
  .divider-text { font-family:'VT323',monospace; font-size:14px; letter-spacing:3px; color:var(--color-text-tertiary); }
  .section-label { font-family:'VT323',monospace; font-size:22px; letter-spacing:3px; color:var(--color-text-secondary); text-transform:uppercase; margin-bottom:1rem; display:flex; align-items:center; gap:8px; }
  .section-label::before { content:'>'; color:#00e5ff; opacity:.7; }
  .section { margin-bottom:2rem; }

  .game-grid { display:grid; grid-template-columns:1fr 1fr; gap:10px; margin-bottom:.5rem; }
  .game-card {
    border:1px solid var(--color-border-tertiary);
    border-radius:6px;
    padding:1rem 1rem 2rem;
    font-size:12px;
    position:relative;
    overflow:hidden;
    transition:border-color .2s, transform .2s;
  }
  .game-card:hover { border-color:var(--color-border-primary); transform: scale(1.01); }

  .game-name { font-weight:700; font-size:15px; margin-bottom:4px; color:var(--color-text-primary); font-family:'VT323',monospace; letter-spacing:2px; }
  .game-twist { font-size:11px; color:#ff003c; opacity:.8; margin-bottom:6px; font-family:'VT323',monospace; letter-spacing:1px; }
  .game-desc { color:var(--color-text-secondary); line-height:1.6; font-size:11px; }

  .badge {
    position:absolute; top:10px; right:10px;
    font-family:'VT323',monospace; font-size:11px; letter-spacing:1px;
    padding:1px 7px; border-radius:3px; border:1px solid;
  }
  .badge.red { color:#ff003c; border-color:#ff003c44; background:#ff003c0a; }
  .badge.cyan { color:#00e5ff; border-color:#00e5ff44; background:#00e5ff0a; }
  .badge.yellow { color:#f0c040; border-color:#f0c04044; background:#f0c0400a; }
  .badge.green { color:#40e040; border-color:#40e04044; background:#40e0400a; }

  .corrupt-bar { position:absolute; bottom:0; left:0; right:0; height:3px; background:linear-gradient(90deg,#ff003c,#00e5ff,#ff003c); background-size:200%; animation:slide 3s linear infinite; opacity:.4; }
  @keyframes slide { from{background-position:0%} to{background-position:200%} }

  .install-block { background:var(--color-background-secondary); border:1px solid var(--color-border-tertiary); border-radius:6px; padding:.8rem 1.1rem; font-size:12px; color:var(--color-text-secondary); margin-top:.5rem; }
  .install-block span.cmd { color:#00e5ff; opacity:.8; }
  .install-block span.cmt { color:var(--color-text-tertiary); }
  .tech-list { display:flex; flex-wrap:wrap; gap:6px; margin-top:.5rem; }
  .tag { font-family:'VT323',monospace; font-size:14px; letter-spacing:1px; padding:2px 10px; border:1px solid var(--color-border-secondary); border-radius:3px; color:var(--color-text-secondary); }
  .tag.hi { border-color:#00e5ff55; color:#00e5ff; opacity:.8; }
  .blink { animation:blink 1.2s step-end infinite; }
  @keyframes blink { 50%{opacity:0} }
  .footer { font-family:'VT323',monospace; font-size:13px; letter-spacing:2px; color:var(--color-text-tertiary); text-align:center; margin-top:1rem; }
  .ascii-logo { font-family:'VT323',monospace; font-size:11px; line-height:1.2; color:var(--color-text-tertiary); opacity:.35; text-align:center; margin:.5rem 0 1.5rem; letter-spacing:1px; white-space:pre; overflow:hidden; }
  .philosophy-line { font-size:12px; line-height:2; color:var(--color-text-secondary); }
  .philosophy-line strong { color:var(--color-text-primary); }
</style>

<div class="readme">
  <div class="header">
    <div class="glitch-title" data-text="RADOGAMES">RADOGAMES</div>
    <div class="tagline">a collection of broken games</div>
  </div>

  <div class="ascii-logo">█▀▀█ ░█▀▀█ ░█─░█ ░█─── ░█▀▀█
█▄▄▀ ░█▄▄█ ░█▄▄█ ░█─── ░█▄▄▀
▀─▀▀ ░█─── ░█──█ ░█▄▄█ ░█─░█  //  C  +  RAYLIB</div>

  <div class="divider"><span class="divider-text">///  GAMES  ///</span></div>

  <div class="section">
    <div class="section-label">Games</div>
    <div class="game-grid">

      <div class="game-card">
        <span class="badge red">SNAKE</span>
        <div class="game-name">🐍 Snake</div>
        <div class="game-twist">// eating food makes you shorter</div>
        <div class="game-desc">Classic snake. Except the apple shrinks you. And the walls are fine. You just lose for no reason eventually.</div>
        <div class="corrupt-bar"></div>
      </div>

      <div class="game-card">
        <span class="badge cyan">MINESWEEPER</span>
        <div class="game-name">💣 MineSweeper</div>
        <div class="game-twist">// the numbers are wrong</div>
        <div class="game-desc">All the mines are placed correctly. The hint numbers are not. Good luck.</div>
        <div class="corrupt-bar"></div>
      </div>

      <div class="game-card">
        <span class="badge yellow">TIC-TAC-TOE</span>
        <div class="game-name">❌ TikTakToe</div>
        <div class="game-twist">// X always loses. always.</div>
        <div class="game-desc">Two-player. Fully functional. The win detection just has a slight bias toward O. Inexplicable.</div>
        <div class="corrupt-bar"></div>
      </div>

      <div class="game-card">
        <span class="badge green">HANGMAN</span>
        <div class="game-name">🪢 Hangman</div>
        <div class="game-twist">// correct guesses add a body part</div>
        <div class="game-desc">You're trying to guess the word. Every right letter gets you closer to death. Every wrong one does nothing.</div>
        <div class="corrupt-bar"></div>
      </div>

    </div>
  </div>

  <div class="divider"><span class="divider-text">///  ABOUT  ///</span></div>

  <div class="section">
    <div class="section-label">Concept</div>
    <div class="philosophy-line">
      Games that <strong>work</strong> — just not correctly.<br><br>
      The physics behave oddly. The rules are off.
      Something familiar is slightly incorrect in a way you can't quite ignore.
    </div>
  </div>

  <div class="divider"><span class="divider-text">///  BUILD  ///</span></div>

  <div class="section">
    <div class="section-label">Build & run</div>
    <div class="install-block">
      <span class="cmt"># requires raylib installed</span><br>
      <span class="cmd">git clone</span> https://github.com/Jam1nna/RadoGames<br>
      <span class="cmd">cd</span> RadoGames<br>
      <span class="cmd">gcc main.c -lraylib -lm -o game && ./game</span>
    </div>
  </div>

  <div class="divider"><span class="divider-text">///  STACK  ///</span></div>

  <div class="tech-list">
    <span class="tag hi">C</span>
    <span class="tag hi">raylib</span>
    <span class="tag">no engine</span>
    <span class="tag">no excuses</span>
    <span class="tag">intentionally wrong</span>
  </div>

  <div class="divider"><span class="divider-text">///  ///</span></div>

  <div class="footer">© RadoGames — all wrongs reserved <span class="blink">█</span></div>
</div>
