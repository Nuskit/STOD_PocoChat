#include "pch.h"

#include <algorithm>
#include <exception>

#include "Poco/ThreadPool.h"

#include "Tools.h"
#include "TaskManager.h"


namespace PocoChatGeneral
{
  std::shared_ptr<TaskManager> TaskManager::Create() 
  { 
    return std::shared_ptr<TaskManager>{ new TaskManager() };
  }

  void TaskManager::RunnableTask::run()
  {
    try
    {
      wrapped_task_->run();
    }
    catch (const std::exception e)
    {
      debug_message("Task exception: ", e.what());
    }
    parent_manager_->finish_task(himself_it_);
  }

  TaskManager::RunnableTask::RunnableTask(const std::shared_ptr<TaskManager>& parent_manager, std::unique_ptr<Poco::Runnable>&& wrapped_task) :
    himself_it_(), wrapped_task_(std::move(wrapped_task)), parent_manager_(parent_manager)
  {

  }

  void TaskManager::RunnableTask::register_himself_link(std::list<std::shared_ptr<RunnableTask>>::iterator himself_it)
  {
    himself_it_ = std::move(himself_it);
  }

  void TaskManager::add_task(std::unique_ptr<Poco::Runnable>&& task)
  {
    auto task_wrapper = std::make_shared<TaskManager::RunnableTask>(shared_from_this(), std::move(task));
    push_task(task_wrapper);

    Poco::ThreadPool::defaultPool().start(*task_wrapper);
  }

  void TaskManager::push_task(const std::shared_ptr<TaskManager::RunnableTask>& task)
  {
    std::lock_guard<decltype(task_locker_)> lock(task_locker_);

    tasks_.push_front(task);
    task->register_himself_link(tasks_.begin());
  }

  void TaskManager::finish_task(const std::list<std::shared_ptr<TaskManager::RunnableTask>>::iterator& task_iter)
  {
    std::lock_guard<decltype(task_locker_)> lock(task_locker_);

    tasks_.erase(task_iter);
  }
}