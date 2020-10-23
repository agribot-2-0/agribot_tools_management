
namespace ToolManager
{
  enum class ErrorCode
  {
    Success = 0
  };
  class ToolManagerTask
  {
  public:
    virtual ErrorCode perform() = 0;
  }
}  // namespace ToolManager