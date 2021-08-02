      .code
      ;; bool equal8_cstr_x86_8(const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2)
      ?equal8_cstr_x86_8@detail@ful@@YA_NPEBE00@Z proc uses rdi rsi
      mov rsi, rcx              ; beg1
      mov rdi, r8               ; beg2
      mov rcx, rdx
      sub rcx, rsi              ; end1 - beg1
      repe cmpsb
      je found_zero
      cmp BYTE PTR [rdi], 0
found_zero:
      sete al
      ret
      ?equal8_cstr_x86_8@detail@ful@@YA_NPEBE00@Z endp
      end
