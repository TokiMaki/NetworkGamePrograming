#include "Scene.h"
#include "Waitscene.h"
#include "GameClient.h"
#include "stdafx.h"
#include "socket_function.h"

WaitScene::WaitScene() {}
WaitScene::WaitScene(SceneNum num, GameClient* const pGameClient) {
    m_SceneNum = num;
    m_pGameClient = pGameClient;
}
WaitScene::~WaitScene() {}

void WaitScene::InitScene() {
    system("cls");

    int retval;

    // 윈속 초기화
    if (m_pGameClient->InitWSA() != true)
        exit(1);

    // socket()
    m_pGameClient->SetSOCKET(socket(AF_INET, SOCK_STREAM, 0));
    if (m_pGameClient->GetSOCKET() == INVALID_SOCKET) err_quit("socket()");

    // connect()
    m_pGameClient->ConnetServer();

    CreateThread(NULL, 0, TestThread, (LPVOID)m_pGameClient->GetSOCKET(), 0, NULL);
}

void WaitScene::Update(float fTimeElapsed) {
    static float WaitTimer = 0.f;
    WaitTimer += fTimeElapsed;
    std::cout << "Waiting";
    for (int i = 0; i < 3; ++i) {
        if (WaitTimer >= i * 0.5) {
            std::cout << ".";
        }
    }
    if (WaitTimer >= 1.5) {
        system("cls");
        WaitTimer = 0;
    }
    std::cout << "\r";
}

DWORD __stdcall WaitScene::TestThread(LPVOID arg)
{
    int retval;
    int len = 0;
    int Msg;
    SOCKET client_sock = (SOCKET)arg;

    while (1) {
        retval = recvn(client_sock, (char*)&len, sizeof(int), 0);
        if (retval == SOCKET_ERROR) {
            err_quit("recv()");
            break;
        }
        else if (retval == 0)
            break;
        len = ntohl(len);

        retval = recvn(client_sock, (char*)&Msg, len, 0);
        if (retval == SOCKET_ERROR) {
            err_quit("recv()");
            break;
        }
        else if (retval == 0)
            break;
        Msg = ntohl(Msg);
        // printf("%d\n", Msg);

        Msg = 0;
        int sendMsg = htonl(Msg);

        int MSG_len = htonl(sizeof(int));
        retval = send(client_sock, (char*)&MSG_len, sizeof(int), 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }

        retval = send(client_sock, (char*)&sendMsg, sizeof(int), 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }
    }
    return 0;
}

