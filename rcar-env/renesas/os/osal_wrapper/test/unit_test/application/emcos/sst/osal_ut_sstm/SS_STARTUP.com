;set trace/line
start log/all           ; start logging...
on error then continue  ; error continue
set unit/all            ; load debug info.. in object file
@reset                  ; reset seq.. 

set reg pc=entry_point	;プログラムカウンタレジスタにmainのアドレスを設定します。

DIRECT ENABLE MEM_64BIT  ; Simulator Command
direct enable simd
