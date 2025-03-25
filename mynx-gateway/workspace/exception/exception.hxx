
namespace exception
{
    [[ nodiscard ]] long exception_filter(
        PEXCEPTION_POINTERS p_exception_pointers
    )
    {
        const auto* context_record = p_exception_pointers->ContextRecord;
        const auto* exception_record = p_exception_pointers->ExceptionRecord;

        char message[ 4096 ];
        sprintf( message,
            encrypt( "Orelia has been stopped due to a fatal error.\n\n"
                "If this is the first time you've seen this error message, restart helga.\n"
                "If this message appears again, follow these steps:\n\n"
                "Rollback any newly downloaded Windows Updates.\n\n"
                "If nothing helps, please contact helga support via the tickets section.\n\n"
                "Technical information:\n"
                "Build type: 0 (built on %s %s)\n"
                "Error code: 0x%08X\n"
                "Address: %p\n\n\n"
                "RSP = 0x%016llX\n"
                "RDI = 0x%016llX\n"
                "RSI = 0x%016llX\n"
                "RBX = 0x%016llX\n"
                "RDX = 0x%016llX\n"
                "RCX = 0x%016llX\n"
                "RAX = 0x%016llX\n"
                "RBP = 0x%016llX\n" ),
            __DATE__, __TIME__,
            p_exception_pointers->ExceptionRecord->ExceptionCode,
            p_exception_pointers->ExceptionRecord->ExceptionAddress,
            context_record->Rsp,
            context_record->Rdi,
            context_record->Rsi,
            context_record->Rbx,
            context_record->Rdx,
            context_record->Rcx,
            context_record->Rax,
            context_record->Rbp
        );

        MessageBoxA( 0, message, encrypt( "Orelia has crashed!" ), MB_ICONERROR | MB_OK );

        return EXCEPTION_CONTINUE_EXECUTION;
    }
}