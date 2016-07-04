#ifndef _LINUX_LINKAGE_H
#define _LINUX_LINKAGE_H

#include <linux/compiler.h>
#include <linux/stringify.h>
#include <linux/export.h>
#include <asm/linkage.h>

/* Some toolchains use other characters (e.g. '`') to mark new line in macro */
#ifndef ASM_NL
#define ASM_NL		 ;
#endif

#ifdef __cplusplus
#define CPP_ASMLINKAGE extern "C"
#else
#define CPP_ASMLINKAGE
#endif

#ifndef asmlinkage
#define asmlinkage CPP_ASMLINKAGE
#endif

#ifndef cond_syscall
#define cond_syscall(x)	asm(				\
	".weak " VMLINUX_SYMBOL_STR(x) "\n\t"		\
	".set  " VMLINUX_SYMBOL_STR(x) ","		\
		 VMLINUX_SYMBOL_STR(sys_ni_syscall))
#endif

#ifndef SYSCALL_ALIAS
#define SYSCALL_ALIAS(alias, name) asm(			\
	".globl " VMLINUX_SYMBOL_STR(alias) "\n\t"	\
	".set   " VMLINUX_SYMBOL_STR(alias) ","		\
		  VMLINUX_SYMBOL_STR(name))
#endif

#define __page_aligned_data	__section(.data..page_aligned) __aligned(PAGE_SIZE)
#define __page_aligned_bss	__section(.bss..page_aligned) __aligned(PAGE_SIZE)

/*
 * For assembly routines.
 *
 * Note when using these that you must specify the appropriate
 * alignment directives yourself
 */
#define __PAGE_ALIGNED_DATA	.section ".data..page_aligned", "aw"
#define __PAGE_ALIGNED_BSS	.section ".bss..page_aligned", "aw"

/*
 * This is used by architectures to keep arguments on the stack
 * untouched by the compiler by keeping them live until the end.
 * The argument stack may be owned by the assembly-language
 * caller, not the callee, and gcc doesn't always understand
 * that.
 *
 * We have the return value, and a maximum of six arguments.
 *
 * This should always be followed by a "return ret" for the
 * protection to work (ie no more work that the compiler might
 * end up needing stack temporaries for).
 */
/* Assembly files may be compiled with -traditional .. */
#ifndef __ASSEMBLY__
#ifndef asmlinkage_protect
# define asmlinkage_protect(n, ret, args...)	do { } while (0)
#endif
#endif

#ifndef __ALIGN
#define __ALIGN		.align 4,0x90
#define __ALIGN_STR	".align 4,0x90"
#endif

#ifdef __ASSEMBLY__

#ifndef LINKER_SCRIPT
#define ALIGN __ALIGN
#define ALIGN_STR __ALIGN_STR

#ifndef ENTRY
#define ENTRY(name) \
	.globl name ASM_NL \
	ALIGN ASM_NL \
	name:
#endif
#endif /* LINKER_SCRIPT */

/* #hwon
	ENTRY가 다른 곳에서 정의되지 않았다면,
	- globl name ASM_NL					-> name으로 전역적인 이름 설정
										ASM_NL : ; 왜냐하면, 몇몇 툴체인 '`' 사용해서
	hwon*/

#ifndef WEAK
#define WEAK(name)	   \
	.weak name ASM_NL   \
	name:
#endif

#ifndef END
#define END(name) \
	.size name, .-name
#endif

/* #hwon
   - .size name, .-name 어셈블러에게 name이 얼마나 많은 space를 사용하는지 알려주기 위해 
	hwon */

/* If symbol 'name' is treated as a subroutine (gets called, and returns)
 * then please use ENDPROC to mark 'name' as STT_FUNC for the benefit of
 * static analysis tools such as stack depth analyzer.
 */
#ifndef ENDPROC
#define ENDPROC(name) \
	.type name, @function ASM_NL \
	END(name)
#endif

#endif

#endif
