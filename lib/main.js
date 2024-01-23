"use strict";
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
var __importStar = (this && this.__importStar) || function (mod) {
    if (mod && mod.__esModule) return mod;
    var result = {};
    if (mod != null) for (var k in mod) if (Object.hasOwnProperty.call(mod, k)) result[k] = mod[k];
    result["default"] = mod;
    return result;
};
Object.defineProperty(exports, "__esModule", { value: true });
const core = __importStar(require("@actions/core"));
const github = __importStar(require("@actions/github"));
function run() {
    var _a, _b;
    return __awaiter(this, void 0, void 0, function* () {
        try {
            const issueCloseMessage = core.getInput('issue-close-message');
            if (!issueCloseMessage) {
                throw new Error('Action must have at least one of issue-close-message set');
            }
            const issueBodyPattern = core.getInput('issue-body-pattern');
            const issueTitlePattern = core.getInput('issue-title-pattern');
            if (!issueBodyPattern && !issueBodyPattern) {
                throw new Error('Action must have at least one of issue-body-pattern or issue-title-pattern set');
            }
            // Get client and context
            const client = new github.GitHub(core.getInput('repo-token', { required: true }));
            const context = github.context;
            const payload = context.payload;
            // Do nothing if it's wasn't being opened or it's not an issue
            if (payload.action !== 'opened' || !payload.issue) {
                return;
            }
            if (!payload.sender) {
                throw new Error('Internal error, no sender provided by GitHub');
            }
            const issue = context.issue;
            const bodyMatches = check(issueBodyPattern, (_a = payload === null || payload === void 0 ? void 0 : payload.issue) === null || _a === void 0 ? void 0 : _a.body);
            const titleMatches = check(issueTitlePattern, (_b = payload === null || payload === void 0 ? void 0 : payload.issue) === null || _b === void 0 ? void 0 : _b.title);
            // Do nothing if no match
            if (!bodyMatches && !titleMatches) {
                return;
            }
            // Comment and close
            yield client.issues.createComment({
                owner: issue.owner,
                repo: issue.repo,
                issue_number: issue.number,
                body: evalTemplate(issueCloseMessage, payload)
            });
            yield client.issues.update({
                owner: issue.owner,
                repo: issue.repo,
                issue_number: issue.number,
                state: 'closed'
            });
        }
        catch (error) {
            core.setFailed(error.message);
            return;
        }
    });
}
function check(patternString, text) {
    const pattern = new RegExp(patternString);
    if (text === null || text === void 0 ? void 0 : text.match(pattern)) {
        return true;
    }
    else {
        return false;
    }
}
function evalTemplate(template, params) {
    return Function(...Object.keys(params), `return \`${template}\``)(...Object.values(params));
}
run();
