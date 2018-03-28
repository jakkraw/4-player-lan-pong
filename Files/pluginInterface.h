#pragma once

class PluginInterface {
public:
	virtual bool initialize() = 0;
	virtual void shutdown() = 0;
};