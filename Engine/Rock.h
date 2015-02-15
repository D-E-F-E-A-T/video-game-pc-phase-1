#include "pch.h"
#include "BaseGridSpace.h"

using namespace Microsoft::WRL;

class Rock : public BaseGridSpace
{
public:
	Rock();
	Rock(int row, int column, float x, float y, ComPtr<ID2D1SolidColorBrush> brush);
	~Rock();

	void Draw(ComPtr<ID2D1DeviceContext1> context);

protected:

private:
};