#!/bin/bash

WORKSPACE=$1
ACTION=$2
MESSAGE=$3

# Available workspaces
WORKSPACES=("cpp-advanced" "python-mastery" "web-development" "algorithms-practice" "projects" "certifications")

show_usage() {
    echo "🚀 Developer Evolution Git Workflow"
    echo "Usage: ./git_workflow.sh <workspace> <action> [message]"
    echo ""
    echo "Workspaces: ${WORKSPACES[*]}"
    echo ""
    echo "Actions:"
    echo "  start     - Begin work session (create branch)"
    echo "  save      - Save progress (commit & push)"
    echo "  finish    - Complete session (merge to main)"
    echo "  status    - Show current status"
    echo "  sync      - Pull latest changes"
    echo ""
    echo "Examples:"
    echo "  ./git_workflow.sh cpp-advanced start"
    echo "  ./git_workflow.sh cpp-advanced save 'Completed day1 exercises'"
    echo "  ./git_workflow.sh python-mastery finish"
    echo "  ./git_workflow.sh projects status"
}

if [ -z "$WORKSPACE" ] || [ -z "$ACTION" ]; then
    show_usage
    exit 1
fi

# Validate workspace
if [[ ! " ${WORKSPACES[@]} " =~ " ${WORKSPACE} " ]]; then
    echo "❌ Invalid workspace: $WORKSPACE"
    echo "Available: ${WORKSPACES[*]}"
    exit 1
fi

case $ACTION in
    "start")
        echo "🚀 Starting work session on $WORKSPACE"
        git checkout main
        git pull origin main
        BRANCH="work/$WORKSPACE-$(date +%Y%m%d-%H%M)"
        git checkout -b "$BRANCH"
        echo "✅ Created branch: $BRANCH"
        echo "📁 Workspace: $WORKSPACE"
        ;;
        
    "save")
        echo "💾 Saving progress for $WORKSPACE"
        git add .
        if [ -n "$MESSAGE" ]; then
            COMMIT_MSG="[$WORKSPACE] $MESSAGE"
        else
            COMMIT_MSG="[$WORKSPACE] Progress update: $(date +%Y-%m-%d)"
        fi
        git commit -m "$COMMIT_MSG"
        git push -u origin HEAD
        echo "✅ Progress saved to GitHub"
        ;;
        
    "finish")
        echo "🎉 Finishing work session for $WORKSPACE"
        git add .
        if [ -n "$MESSAGE" ]; then
            COMMIT_MSG="[$WORKSPACE] $MESSAGE"
        else
            COMMIT_MSG="[$WORKSPACE] Completed session: $(date +%Y-%m-%d)"
        fi
        git commit -m "$COMMIT_MSG"
        git push origin HEAD
        
        # Merge back to main
        git checkout main
        git merge --no-ff HEAD@{1}
        git push origin main
        echo "✅ $WORKSPACE session completed and merged to main"
        ;;
        
    "status")
        echo "📊 Status for $WORKSPACE workspace"
        echo "Current branch: $(git branch --show-current)"
        echo "Recent commits in $WORKSPACE:"
        git log --oneline --grep="\\[$WORKSPACE\\]" -10
        ;;
        
    "sync")
        echo "🔄 Syncing latest changes"
        git checkout main
        git pull origin main
        echo "✅ Repository synced"
        ;;
        
    *)
        echo "❌ Unknown action: $ACTION"
        show_usage
        exit 1
        ;;
esac
