#pragma once

struct Hooks
{
	// 주의: 프로그램 실행 도중에 훅을 설치할 경우 멀티스레드 문제로 크래시할 수 있음.
	static void Install();
	static void Uninstall();
};