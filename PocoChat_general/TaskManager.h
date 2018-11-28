#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <mutex>
#include <memory>
#include <list>

#include "Poco/Runnable.h"

namespace PocoChatGeneral
{
  class TaskManager : public std::enable_shared_from_this<TaskManager>
  {
    class RunnableTask : public Poco::Runnable
    {
    public:
      RunnableTask(const std::shared_ptr<TaskManager>& parent_manager, std::unique_ptr<Poco::Runnable>&& wrapped_task);
      void register_himself_link(std::list<std::shared_ptr<RunnableTask>>::iterator himself_it);

      virtual void run() override;
    private:
      std::list<std::shared_ptr<RunnableTask>>::iterator himself_it_;
      std::unique_ptr<Poco::Runnable> wrapped_task_;
      const std::shared_ptr<TaskManager> parent_manager_;
    };
    friend class RunnableTask;
  public:
    static std::shared_ptr<TaskManager> Create();
    TaskManager(const TaskManager&) = delete;
    TaskManager(TaskManager&&) = delete;
    TaskManager& operator=(const TaskManager&) = delete;
    TaskManager& operator=(TaskManager&&) = delete;
    ~TaskManager() = default;

    void add_task(std::unique_ptr<Poco::Runnable>&& task);
  private:
    TaskManager() = default;
    void push_task(const std::shared_ptr<RunnableTask>& task);
    void finish_task(const std::list<std::shared_ptr<RunnableTask>>::iterator& task_iter);

    std::mutex task_locker_;
    std::list<std::shared_ptr<RunnableTask>> tasks_;
  };
}

#endif
