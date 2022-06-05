#pragma once

#include "CoreMinimal.h"
#include "Logger.generated.h"

UENUM(BlueprintType)
enum class ELogLevel : uint8
{
	TRACE UMETA(DisplayName = "Trace"),
	DEBUG UMETA(DisplayName = "Debug"),
	INFO UMETA(DisplayName = "Info"),
	WARNING UMETA(DisplayName = "Warning"),
	ERROR UMETA(DisplayName = "Error")
};

UENUM(BlueprintType)
enum class ELogOutput : uint8
{
	ALL UMETA(DisplayName = "All levels"),
	OUTPUT_LOG UMETA(DisplayName = "Output log"),
	SCREEN UMETA(DisplayName = "Screen")
};

UCLASS()
class SWD_API ULogger : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:
	/**
	* Log - prints a message to all the log outputs with a specific color
	* @param Level {@see ELogLevel} affects color of log
	* @param FString the message for display
	*/
	static void Log(ELogLevel Level, FString Message);
	
	static void LogError(FString Message);
	/**
	* Log - prints a message to all the log outputs with a specific color
	* @param Level {@see ELogLevel} affects color of log
	* @param FString the message for display
	* @param ELogOutput - All, Output Log or Screen
	*/
	static void Log(ELogLevel Level, FString Message, ELogOutput LogOutput);
};
