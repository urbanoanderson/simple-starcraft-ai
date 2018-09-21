# Sobre

Software desenvolvido como projeto da disciplina de Agentes Inteligentes do CIn-UFPE (2015).

Consiste em uma inteligência artificial para controlar uma civilização no jogo de estratégia Starcraft.

# Instalação

1) Entre no site: "http://www.cin.ufpe.br/~if703/BWAPI/" e baixe
o visual studio 2008 e instale o 'visual c++ express 2008'

2) No mesmo site tem uma caixa de texto vazia. Digite "a3pi" e dê
enter para baixar o arquivo "Starcraft A3PI 3.7.rar" com o jogo
e a api instalada.

3) Extraia o arquivo e coloque a pasta "Starcraft" no drive "C:\"

4) Abra o arquivo "C:\Starcraft\bwapi-data\bwapi.ini" 
em [ai] coloque:
"ai     = bwapi-data\AI\Plata_o_Plomo.dll, bwapi-data\AI\A3parada.dll"
em [auto_menu] coloque:
"auto_menu = SINGLE_PLAYER"
e "map = maps\BroodWar\Gas Multi 2.scm"

5) Abra o lancador do jogo como administrador
"C:\Starcraft\BWAPI 3.7.2\Chaoslauncher\Chaoslauncher - MultiInstance.exe"
Na primeira vez vá na aba settings e coloque o installpath como
"C:\Starcraft\" e clique em ok. Ele vai reiniciar o lancador.
Depois na aba plugins marque apenas a primeira e a terceira opção
e do lado do botão start coloque a opção ""Starcraft Multi-Instance".

6) Aperte start para rodar o jogo. Lembrando que deve ter a dll da nossa ia
"C:\Starcraft\bwapi-data\AI\Plata_o_Plomo.dll"
