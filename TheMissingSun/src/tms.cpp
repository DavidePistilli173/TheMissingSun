#include "../include/tms.hpp"

namespace tms
{
    /***************** FUNCTIONS *****************/
    void stringCopy(char* destination, const char* source)
    {
        int i = 0;
        /* Copy the source string. Truncate it if it is too long. */
        while (i < (UNIFORM_NAME_LEN - 1) && source[i] != 0)
        {
            destination[i] = source[i];
            ++i;
        }
        /* Null terminate the string. */
        destination[i] = 0;
    }

    void GLAPIENTRY openGLError(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam)
    {
        /* Keep track of the number of the current error. */
        static int errorNum = 0;
        ++errorNum;
        /* Desired minimum error level to be printed. */
        const ErrorSeverity MIN_SEVERITY = ErrorSeverity::LOW;
        ErrorSeverity currentSeverity = ErrorSeverity::NOTIFICATION;

        /* Set the error strings. */
        std::string errorType, errorSeverity;

        /* Set the error type string. */
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:
            errorType = "API ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            errorType = "DEPRECATED BEHAVIOUR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            errorType = "UNDEFINED BEHAVIOUR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            errorType = "PORTABILITY ISSUE";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            errorType = "PERFORMANCE ISSUE";
            break;
        case GL_DEBUG_TYPE_MARKER:
            errorType = "STREAM ANNOTATION";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            errorType = "GROUP PUSHING";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            errorType = "GROUP POPPING";
            break;
        case GL_DEBUG_TYPE_OTHER:
            errorType = "OTHER";
            break;
        }

        /* Set the error severity string. */
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            errorSeverity = "HIGH";
            currentSeverity = ErrorSeverity::HIGH;
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            errorSeverity = "MEDIUM";
            currentSeverity = ErrorSeverity::MEDIUM;
            break;
        case GL_DEBUG_SEVERITY_LOW:
            errorSeverity = "LOW";
            currentSeverity = ErrorSeverity::LOW;
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            errorSeverity = "NOTIFICATION";
            currentSeverity = ErrorSeverity::NOTIFICATION;
            break;
        }

        if (currentSeverity >= MIN_SEVERITY)
        {
            printf("OpenGL error %d:\n   type = %s,\n   severity = %s,\n   message = %s\n\n",
                errorNum,
                errorType.c_str(),
                errorSeverity.c_str(),
                message);
        }
    }
}