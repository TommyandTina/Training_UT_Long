;set trace/line
start log/all           ; start logging...
on error then continue  ; error continue
set unit/all            ; load debug info.. in object file
@reset                  ; reset seq.. 

set reg pc=entry_point	;�v���O�����J�E���^���W�X�^��main�̃A�h���X��ݒ肵�܂��B

DIRECT ENABLE MEM_64BIT  ; Simulator Command
direct enable simd
