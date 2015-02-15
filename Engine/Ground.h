#include "pch.h"
#include "BaseGridSpace.h"

using namespace Microsoft::WRL;

class Ground : public BaseGridSpace
{
public:
	Ground();
	Ground(int row, int column, float x, float y, ComPtr<ID2D1SolidColorBrush> brush);
	~Ground();

	void Draw(ComPtr<ID2D1DeviceContext1> context);

protected:

private:
};