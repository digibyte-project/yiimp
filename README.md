yiimp for digibyte/randomx
==========================

(c) barrystyle 2020.


provided here as a reference implementation; 
yiimp setup to accept a monero-type stratum header, for use with digibyte/randomx.

allowing native xmrig/rx0 use with a satoshi-based currency (the first).


what this does:
  * correctly validates shares
  * correctly validates and submits blocks

what this doesnt do:
  * track % of valid shares
  * does not handle payouts
  * track duplicate shares
  * everything else

what could be improved:
  * handling of the randomx cache/seed
  * continuity of communications between pool/miner


direct your complaints/'how do i set this up' to /dev/null.
improvements and PR's always welcome.
reuse is welcome as long as this message is included/copyrights are left intact.

feel free to come say hi on discord (barrystyle#3615) or telegram (@barrystyle).


