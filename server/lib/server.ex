defmodule Server do
  use Application

  def start(_type, _args) do
    GlobalSupervisor.start_link([])
    commandParse()
  end

  def commandParse() do
    case IO.gets("> ") do
      "stop\n" ->
      Router.stop()
      {:ok, :normal}
      _->
        commandParse()
    end
  end
end
