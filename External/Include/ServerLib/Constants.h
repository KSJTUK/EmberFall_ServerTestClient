#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Constant.h
// 2025 - 01 - 14 (설명 추가 날짜)
//      김성준: 여러 상수, 전역 객체 선언
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline constexpr SessionIdType INVALID_SESSION_ID = std::numeric_limits<SessionIdType>::max();

inline constexpr size_t MAX_KEY_SIZE = 256;
inline constexpr size_t MAX_BUF_SIZE = std::numeric_limits<unsigned short>::max();
inline constexpr size_t BUF_NETWORK_RECV_SIZE = 65535;
inline constexpr NetworkObjectIdType OBJECT_ID_START = INVALID_SESSION_ID + 1;

inline const uint32_t HARDWARE_CONCURRENCY = std::thread::hardware_concurrency();

enum class IOType : uint32_t {
    SEND,
    RECV,
    CONNECT,
    DISCONNECT,
    ACCEPT
};

enum class CollisionState : BYTE {
    NONE,
    ENTER,
    STAY,
    EXIT
};

extern std::unique_ptr<LogConsole> gLogConsole;