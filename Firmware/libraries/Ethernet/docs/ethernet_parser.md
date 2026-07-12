# EthernetParser — Parte 1

Interpreta quadros Ethernet sem VLAN.

- extrai MAC de destino;
- extrai MAC de origem;
- identifica EtherType;
- cria um `BufferView` zero-copy para o payload;
- valida comprimento mínimo e máximo.

A Parte 2 implementará VLAN IEEE 802.1Q.
